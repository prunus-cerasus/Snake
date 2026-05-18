#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <cstdint>
#include <vector>
#include <random>
#include <variant>

#include "core/SDLState.hpp"
#include "core/Timer.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/state/Type.hpp"
#include "core/state/specs/gameplay.hpp"

namespace state {
    struct Gameplay {
        Gameplay();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
        void logic(int64_t p_deltaTime, state::Type& p_nextState);
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
        void detectCollision(state::Type& p_nextState);

        std::vector<Entity> entities;
        std::vector<size_t> inactiveEntityIds;
        std::vector<size_t> consumableIds;
        size_t rockCount;

        Player player;
        Timer movePlayerTimer;
        bool hasConsumed;

        std::mt19937 mt_gen;
        std::uniform_int_distribution<> distrib_w;
        std::uniform_int_distribution<> distrib_h;
    };
}

#endif // GAMEPLAY_HPP