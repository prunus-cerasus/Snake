#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <variant>
#include <vector>
#include <random>
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/specs.hpp"
#include "core/Timer.hpp"

#include "core/SDLState.hpp"

struct Game;

namespace state {
    struct Empty { Empty() = default; };

    struct Menu{
        void handleEvents(Game*) {}
        void logic(Game*) {}
        void render(Game*) {}
    };
    struct Gameplay {
        Gameplay();

        void handleEvents(const bool* p_keyState);
        void logic(int64_t p_deltaTime, bool& p_isRunning);
        void render(SDLState& p_sdl);

        void changePlayerDirection(const bool* p_keyState);
        inline void deactivateEntity(size_t p_entityIdx);
        inline size_t activateEntity(EntityType p_type);
        inline void increasePlayer();
        inline void decreasePlayer();
        void movePlayerBody();
        void movePlayerHead();
        void movePlayer();
        void drawEntity(SDLState& p_sdl, const Entity& p_entity);
        void randomizeConsumable(size_t p_consumableIdx);
        void generateConsumable(EntityType p_type);
        void removeConsumable(size_t p_consumableIdx);
        void detectCollision(bool& p_isRunning);

        std::vector<Entity> entities;
        std::vector<size_t> inactiveEntityIds;
        std::vector<size_t> consumableIds;

        Player player;
        Timer movePlayerTimer;
        bool hasConsumed;

        std::mt19937 mt_gen;
        std::uniform_int_distribution<> distrib_w;
        std::uniform_int_distribution<> distrib_h;
    };
    struct GameOver {
        void handleEvents(Game*) {}
        void logic(Game*) {}
        void render(Game*) {}
    };
}

struct Game {
    Game();
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    ~Game();

    void changeState(std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver> p_newState);

    void handleEvents();
    void logic();
    void render();

    void loop();

    SDLState sdl;

    bool isRunning;
    int64_t deltaTime;
    int64_t currentTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver> state;
};

#endif // GAME_HPP