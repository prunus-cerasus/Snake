#ifndef SPECS_GAMEPLAY_HPP
#define SPECS_GAMEPLAY_HPP

#include <cstdint>

#include "SDL3/SDL.h"

namespace state::specs::gameplay {
    constexpr int32_t WIDTH {40};
    constexpr int32_t HEIGHT {30};
    constexpr float CELL_SIDE {20.0f};
    constexpr int64_t MOVEMENT_TIME {60'000'000};
    constexpr int32_t PLAYERSIZE_PER_ROCK {1};
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

#endif // SPECS_GAMEPLAY_HPP