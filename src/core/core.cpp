#include <iostream>

#include "SDL3/SDL.h"

#include "core/SDLState.hpp"
#include "core/Timer.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"

namespace gamespecs {
    constexpr int WIDTH {40};
    constexpr int HEIGHT {30};
    constexpr float CELL_SIDE {20.0f};
    constexpr int64_t MOVEMENT_TIME {600'000'000};

    constexpr SDL_Color DRAWCOLOR_SNAKE_HEAD {
        .r = 0x00,
        .g = 0xff,
        .b = 0x00,
        .a = 0xff,
    };

    constexpr SDL_Color DRAWCOLOR_SNAKE_BODY {
        .r = 0x00,
        .g = 0x9f,
        .b = 0x00,
        .a = 0xff,
    };

    constexpr SDL_Color DRAWCOLOR_APPLE {
        .r = 0xff,
        .g = 0x00,
        .b = 0x00,
        .a = 0xff,
    };

    constexpr SDL_Color DRAWCOLOR_ROCK {
        .r = 0xa0,
        .g = 0xa0,
        .b = 0xa0,
        .a = 0xff,
    };
};

void drawEntity(SDLState& p_sdl, const Entity& p_obj) {
    SDL_Color dcolor{};
    switch(p_obj.type) {
        case EntityType::PlayerHead:
            dcolor = gamespecs::DRAWCOLOR_SNAKE_HEAD;
        break;

        case EntityType::PlayerBody:
            dcolor = gamespecs::DRAWCOLOR_SNAKE_BODY;
        break;

        case EntityType::Apple:
            dcolor = gamespecs::DRAWCOLOR_APPLE;
        break;

        case EntityType::Rock:
            dcolor = gamespecs::DRAWCOLOR_ROCK;
        break;
    }

    SDL_SetRenderDrawColor(p_sdl.getRenderer(), dcolor.r, dcolor.g, dcolor.b, dcolor.a);

    SDL_FRect dest{
        .x = gamespecs::CELL_SIDE*p_obj.pos.x+1,
        .y = gamespecs::CELL_SIDE*p_obj.pos.y+1,
        .w = gamespecs::CELL_SIDE-1,
        .h = gamespecs::CELL_SIDE-1,
    };
    SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
}

struct GameState {
    bool isRunning;
    int64_t deltaTime;
    int64_t previousTime;

    const bool* keyState;
    int numkeys;

    std::vector<Entity> entities;
    Player player;
    Timer movePlayer;

    GameState() : isRunning(true), deltaTime(0L), previousTime(0L), keyState(SDL_GetKeyboardState(&numkeys)), movePlayer(gamespecs::MOVEMENT_TIME) {
        entities.emplace_back(0, 0, EntityType::PlayerHead);
    }

    void increasePlayer() {
        entities.emplace_back(-1, -1, EntityType::PlayerBody);
        player.body.emplace_back(entities.size()-1);
    }
};

void logic(SDLState& p_sdl, GameState& p_game) {
    p_sdl.getWindow();
    SDL_Time currentTime;
    SDL_GetCurrentTime(&currentTime);
    if (p_game.previousTime == 0) p_game.previousTime = currentTime;
    p_game.deltaTime = currentTime - p_game.previousTime;
    p_game.previousTime = currentTime;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_game.isRunning = false;
    }

    if (p_game.movePlayer.step(p_game.deltaTime)) {
        p_game.movePlayer.reset();
        if (p_game.player.direction == PlayerDirection::Up || p_game.player.direction == PlayerDirection::Down) {
            int dir = -p_game.keyState[SDL_SCANCODE_A];
            dir += p_game.keyState[SDL_SCANCODE_D];

            if (dir == -1) p_game.player.direction = PlayerDirection::Left;
            else if (dir == 1) p_game.player.direction = PlayerDirection::Right;
        } else {
            int dir = -p_game.keyState[SDL_SCANCODE_W];
            dir += p_game.keyState[SDL_SCANCODE_S];

            if (dir == -1) p_game.player.direction = PlayerDirection::Up;
            else if (dir == 1) p_game.player.direction = PlayerDirection::Down;
        }

        auto GetBodyReversed = [&](size_t l_index) -> Entity& {
            return p_game.entities[p_game.player.body[p_game.player.body.size()-l_index-1]];
        };
        switch (p_game.player.direction) {
            case PlayerDirection::Up: {
                if (p_game.player.body.size() > 0) for (size_t i{}; i < p_game.player.body.size()-1; i++) {
                    GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
                }
                if (p_game.player.body.size() > 0)
                    GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
                p_game.entities[p_game.player.head].pos.y--;
                break;
            }
            case PlayerDirection::Down: {
                if (p_game.player.body.size() > 0) for (size_t i{}; i < p_game.player.body.size()-1; i++) {
                    GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
                }
                if (p_game.player.body.size() > 0)
                    GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
                p_game.entities[p_game.player.head].pos.y++;
                break;
            }
            case PlayerDirection::Left: {
                if (p_game.player.body.size() > 0) for (size_t i{}; i < p_game.player.body.size()-1; i++) {
                    GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
                }
                if (p_game.player.body.size() > 0)
                    GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
                p_game.entities[p_game.player.head].pos.x--;
                break;
            }
            case PlayerDirection::Right: {
                if (p_game.player.body.size() > 0) for (size_t i{}; i < p_game.player.body.size()-1; i++) {
                    GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
                }
                if (p_game.player.body.size() > 0)
                    GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
                p_game.entities[p_game.player.head].pos.x++;
                break;
            }
        
        }
    }
}

void render(SDLState& p_sdl, GameState& p_game) {
    SDL_SetRenderDrawColor(p_sdl.getRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(p_sdl.getRenderer());

    for (const auto& entity : p_game.entities) {
        drawEntity(p_sdl, entity);
    }

    SDL_RenderPresent(p_sdl.getRenderer());
}

void loop(SDLState& p_sdl) {
    GameState game;
    game.entities[game.player.head].pos = {.x = gamespecs::WIDTH/2, .y = gamespecs::HEIGHT/2};
    // game.entities.emplace_back(1, 1, EntityType::Apple);
    // for (int i = 0; i < 6; i++) game.increasePlayer();

    while (game.isRunning) {
        logic(p_sdl, game);
        render(p_sdl, game);
    }
}

void core() {
    SDLState sdl;
    if (!sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return;
    }
    loop(sdl);
}