#include <iostream>
#include <cstdint>
#include <vector>

#include "SDL3/SDL.h"
#include "core/SDLState.hpp"

namespace gamespecs {
    // constexpr int WIDTH {40};
    // constexpr int HEIGHT {30};
    constexpr float CELL_SIDE {20.0f};
    constexpr int64_t MOVEMENT_TIME {600'000'000};
};

struct vec2 {
    int x;
    int y;
};

enum class EntityType {
    PlayerHead,
    PlayerBody,
    Apple,
    Stone,
};

struct Entity {
    vec2 pos;
    EntityType type;

    Entity(int p_x = 0, int p_y = 0, EntityType p_type = EntityType::PlayerHead) : pos({p_x, p_y}), type(p_type) {

    }

    void draw(SDLState& p_sdl) {
        SDL_FRect dest{
            .x = gamespecs::CELL_SIDE*pos.x+1,
            .y = gamespecs::CELL_SIDE*pos.y+1,
            .w = gamespecs::CELL_SIDE-1,
            .h = gamespecs::CELL_SIDE-1,
        };
        SDL_RenderFillRect(p_sdl.get_renderer(), &dest);
    }
};

enum class PlayerDirection {
    Up,
    Left,
    Down,
    Right,
};

struct Player {
    size_t head;
    std::vector<size_t> body;
    PlayerDirection direction;

    Player(size_t p_head = 0, size_t p_body_size = 0) : head(p_head), body(p_body_size), direction(PlayerDirection::Up) {

    }
};

class Timer {
    int64_t timer;
    int64_t interval;
    bool isDone;
public:
    Timer(int64_t p_interval) : timer(0L), interval(p_interval), isDone(false) {}
    void Reset() {
        timer -= interval;
        isDone = false;
    }

    bool Step(int64_t p_deltaTime) {
        timer += p_deltaTime;
        if (timer >= interval) {
            isDone = true;
        }
        return isDone;
    }
};

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
    p_sdl.get_window();
    SDL_Time currentTime;
    SDL_GetCurrentTime(&currentTime);
    if (p_game.previousTime == 0) p_game.previousTime = currentTime;
    p_game.deltaTime = currentTime - p_game.previousTime;
    p_game.previousTime = currentTime;

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_game.isRunning = false;
    }

    if (p_game.movePlayer.Step(p_game.deltaTime)) {
        p_game.movePlayer.Reset();
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
    SDL_SetRenderDrawColor(p_sdl.get_renderer(), 0, 0, 0, 0);
    SDL_RenderClear(p_sdl.get_renderer());

    SDL_SetRenderDrawColor(p_sdl.get_renderer(), 0, 150, 0, 0);
    for (size_t snake_body_idx : p_game.player.body) {
        p_game.entities[snake_body_idx].draw(p_sdl);
    }

    SDL_SetRenderDrawColor(p_sdl.get_renderer(), 0, 255, 0, 0);
    p_game.entities[p_game.player.head].draw(p_sdl);


    SDL_RenderPresent(p_sdl.get_renderer());
}

void loop(SDLState& p_sdl) {
    GameState game;
    game.entities[game.player.head].pos = {.x = 20, .y = 15};

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