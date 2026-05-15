#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <variant>

#include "core/SDLState.hpp"

namespace state {
    struct Empty{ Empty() = default; };
    struct Menu;
    struct Gameplay;
    struct GameOver;
}

struct Game {
    Game();
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    ~Game();

    void changeState(std::variant<state::Empty, state::Menu*, state::Gameplay*, state::GameOver*> p_newState);

    void handleEvents();
    void logic();
    void render();

    SDLState sdl;

    bool isRunning;
    int64_t deltaTime;
    int64_t currentTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::variant<state::Empty, state::Menu*, state::Gameplay*, state::GameOver*> state;
};

#include <vector>
#include <random>
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/specs.hpp"
#include "core/Timer.hpp"

struct GameState {
    bool isRunning;
    int64_t deltaTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::vector<Entity> entities;
    std::vector<size_t> inactiveEntityIds;
    std::vector<size_t> consumableIds;

    Player player;
    Timer movePlayerTimer;
    bool hasConsumed;


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
        hasConsumed(false),
        mt_gen(rd()),
        distrib_w(0, gamespecs::WIDTH-1),
        distrib_h(0, gamespecs::HEIGHT-1)
        {
        entities.emplace_back(0, 0, EntityType::PlayerHead);
    }
};

namespace state {
    struct Menu{
        void handleEvents(Game*) {}
        void logic(Game*) {}
        void render(Game*) {}
    };
    struct Gameplay {
        void handleEvents(Game* p_game);
        void logic(Game* p_game);
        void render(Game* p_game);

        GameState state;

    };
    struct GameOver {
        void handleEvents(Game*) {}
        void logic(Game*) {}
        void render(Game*) {}
    };
}

#endif // GAME_HPP