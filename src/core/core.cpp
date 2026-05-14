#include <iostream>
#include <random>

#include "SDL3/SDL.h"

#include "core/SDLState.hpp"
#include "core/Timer.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"

#include "core/specs.hpp"

struct GameState {
    bool isRunning;
    int64_t deltaTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::vector<Entity> entities;
    Player player;
    Timer movePlayerTimer;

    std::random_device rd;
    std::mt19937 mt_gen;
    std::uniform_int_distribution<> distrib_w;
    std::uniform_int_distribution<> distrib_h;

    GameState()
        : isRunning(true),
        deltaTime(0L),
        previousTime(0L),
        keyState(SDL_GetKeyboardState(&numkeys)),
        movePlayerTimer(gamespecs::MOVEMENT_TIME),
        mt_gen(rd()),
        distrib_w(0, gamespecs::WIDTH-1),
        distrib_h(0, gamespecs::HEIGHT-1) {
        entities.emplace_back(0, 0, EntityType::PlayerHead);
    }

    void increasePlayer() {
        entities.emplace_back(-1, -1, EntityType::PlayerBody);
        player.body.emplace_back(entities.size()-1);
    }
};

void changePlayerDirection(GameState& p_game) {
    vec2 dir{.x = 0, .y = 0};
    dir.x += p_game.keyState[SDL_SCANCODE_D];
    dir.x -= p_game.keyState[SDL_SCANCODE_A];
    dir.y += p_game.keyState[SDL_SCANCODE_S];
    dir.y -= p_game.keyState[SDL_SCANCODE_W];

    switch (p_game.player.direction) {
        case PlayerDirection::Up:
        case PlayerDirection::Down: {
            if (dir.x == 1) p_game.player.nextDirection = PlayerDirection::Right;
            else if (dir.x == -1) p_game.player.nextDirection = PlayerDirection::Left;
            break;
        }

        case PlayerDirection::Left:
        case PlayerDirection::Right: {
            if (dir.y == 1) p_game.player.nextDirection = PlayerDirection::Down;
            else if (dir.y == -1) p_game.player.nextDirection = PlayerDirection::Up;
            break;
        }
    }

}

void movePlayerBody(GameState& p_game) {
    if (p_game.player.body.size() > 0) {
        auto GetBodyReversed = [&](size_t l_index) -> Entity& {
            return p_game.entities[p_game.player.body[p_game.player.body.size()-l_index-1]];
        };

        for (size_t i{}; i < p_game.player.body.size()-1; i++) {
            GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
        }
        GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
    }
}

void movePlayerHead(GameState& p_game) {
    switch (p_game.player.direction) {
        case PlayerDirection::Up:
            p_game.entities[p_game.player.head].pos.y--;
            break;
        case PlayerDirection::Down:
            p_game.entities[p_game.player.head].pos.y++;
            break;
        case PlayerDirection::Left:
            p_game.entities[p_game.player.head].pos.x--;
            break;
        case PlayerDirection::Right:
            p_game.entities[p_game.player.head].pos.x++;
            break;
    }
}

void movePlayer(GameState& p_game) {
    p_game.player.direction = p_game.player.nextDirection;

    movePlayerBody(p_game);
    movePlayerHead(p_game);
    
    if (
        p_game.entities[p_game.player.head].pos.x < 0 ||
        p_game.entities[p_game.player.head].pos.x >= gamespecs::WIDTH ||
        p_game.entities[p_game.player.head].pos.y < 0 ||
        p_game.entities[p_game.player.head].pos.y >= gamespecs::HEIGHT
    ) p_game.isRunning = false;
}

void drawEntity(SDLState& p_sdl, const Entity& p_obj) {
    SDL_Color dcolor{gamespecs::DRAWCOLOR[static_cast<size_t>(p_obj.type)]};

    SDL_SetRenderDrawColor(p_sdl.getRenderer(), dcolor.r, dcolor.g, dcolor.b, dcolor.a);

    SDL_FRect dest{
        .x = static_cast<float>(gamespecs::CELL_SIDE*p_obj.pos.x+1),
        .y = static_cast<float>(gamespecs::CELL_SIDE*p_obj.pos.y+1),
        .w = static_cast<float>(gamespecs::CELL_SIDE-1),
        .h = static_cast<float>(gamespecs::CELL_SIDE-1),
    };
    SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
}

void handleEvents([[maybe_unused]] SDLState& p_sdl, GameState& p_game) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_game.isRunning = false;
    }

    changePlayerDirection(p_game);
}

void generateApple(GameState& p_game) {
    Entity apple(p_game.entities[p_game.player.head]);
    apple.type = EntityType::Apple;

    while (apple.pos == p_game.entities[p_game.player.head].pos) {
        apple.pos.x = p_game.distrib_w(p_game.mt_gen);
        apple.pos.y = p_game.distrib_h(p_game.mt_gen);
    }

    for (Entity& ent : p_game.entities) {
        if (ent.type == EntityType::Inactive) {
            ent = apple;
            return;
        }
    }

    p_game.entities.emplace_back(apple);
}

void detectCollision(GameState& p_game) {
    Entity& playerHead {p_game.entities[p_game.player.head]};
    for (size_t i{}; i < p_game.entities.size(); i++) {
        if (i == p_game.player.head) continue;

        Entity& ent {p_game.entities[i]};
        if (ent.type == EntityType::Inactive) continue;

        if (playerHead.pos == ent.pos) {
            switch (ent.type) {
                case EntityType::PlayerBody:
                    p_game.isRunning = false;
                break;

                case EntityType::Apple: {
                    ent.type = EntityType::Inactive;
                    p_game.increasePlayer();
                    generateApple(p_game);
                    break;
                }

                case EntityType::Rock:
                break;

                default:
            }
        }
    }
}

void logic([[maybe_unused]] SDLState& p_sdl, GameState& p_game) {
    SDL_Time currentTime;
    SDL_GetCurrentTime(&currentTime);
    if (p_game.previousTime == 0) p_game.previousTime = currentTime;
    p_game.deltaTime = currentTime - p_game.previousTime;
    p_game.previousTime = currentTime;

    if (p_game.movePlayerTimer.step(p_game.deltaTime)) {
        p_game.movePlayerTimer.reset();
        movePlayer(p_game);
        detectCollision(p_game);
    }
}

void render(SDLState& p_sdl, GameState& p_game) {
    SDL_SetRenderDrawColor(p_sdl.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(p_sdl.getRenderer());
    SDL_SetRenderDrawColor(
        p_sdl.getRenderer(),
        gamespecs::BG_COLOR.r,
        gamespecs::BG_COLOR.g,
        gamespecs::BG_COLOR.b,
        gamespecs::BG_COLOR.a
    );

    const SDL_FRect bg_dest {
        .x = 0.0f,
        .y = 0.0f,
        .w = static_cast<float>(specs::LOGICAL_WIDTH),
        .h = static_cast<float>(specs::LOGICAL_HEIGHT),
    };

    SDL_RenderFillRect(p_sdl.getRenderer(), &bg_dest);

    for (const auto& entity : p_game.entities) {
        drawEntity(p_sdl, entity);
    }

    SDL_RenderPresent(p_sdl.getRenderer());
}

void loop(SDLState& p_sdl) {
    GameState game;
    game.entities[game.player.head].pos = {.x = gamespecs::WIDTH/2, .y = gamespecs::HEIGHT/2};
    generateApple(game);
    // for (int32_t i = 0; i < 10; i++) game.increasePlayer();

    while (game.isRunning) {
        handleEvents(p_sdl, game);
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