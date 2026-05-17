#ifndef SPECS_HPP
#define SPECS_HPP

#include "SDL3/SDL.h"

namespace gamespecs {
    constexpr int32_t WIDTH {40};
    constexpr int32_t HEIGHT {30};
    constexpr float CELL_SIDE {20.0f};
    constexpr int64_t MOVEMENT_TIME {40'000'000};
    constexpr int32_t PLAYERSIZE_PER_ROCK {4};

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
};

namespace specs {
    constexpr int LOGICAL_WIDTH {800};
    constexpr int LOGICAL_HEIGHT {600};
    constexpr const char* TITLE {"Program"};
    constexpr int FLAGS {SDL_WINDOW_RESIZABLE};
    constexpr int MAX_BUFFER_COUNT {4};
}

#endif // SPECS_HPP