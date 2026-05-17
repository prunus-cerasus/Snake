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

namespace state {
    namespace specs {
        namespace gameplay {
            constexpr int32_t WIDTH {40};
            constexpr int32_t HEIGHT {30};
            constexpr float CELL_SIDE {20.0f};
            constexpr int64_t MOVEMENT_TIME {40'000'000};
            constexpr int32_t PLAYERSIZE_PER_ROCK {4};
            constexpr int32_t MAX_RANDOM_LOOP_SIZE {256};

            constexpr SDL_Color BG_COLOR {
                .r = 0x30,
                .g = 0x30,
                .b = 0x30,
                .a = 0xff,
            };

            constexpr SDL_Color DRAWCOLOR[] {
                {
                    .r = 0x00,
                    .g = 0xff,
                    .b = 0x00,
                    .a = 0xff,
                },
                {
                    .r = 0x00,
                    .g = 0x9f,
                    .b = 0x00,
                    .a = 0xff,
                },
                {
                    .r = 0xff,
                    .g = 0x00,
                    .b = 0x00,
                    .a = 0xff,
                },
                {
                    .r = 0xaf,
                    .g = 0xaf,
                    .b = 0xaf,
                    .a = 0xff,
                },
            };
        }
    }

    struct Gameplay {
        Gameplay();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
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