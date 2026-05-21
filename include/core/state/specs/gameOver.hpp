#ifndef SPECS_MENU_HPP
#define SPECS_MENU_HPP

#include <filesystem>

#include "SDL3/SDL.h"

namespace state::specs::gameOver {
    constexpr SDL_Color BG_COLOR {
        .r = 0xa0,
        .g = 0x40,
        .b = 0x40,
        .a = 0xff,
    };

    constexpr SDL_Color TEXT_COLOR {
        .r = 0xff,
        .g = 0xff,
        .b = 0xff,
        .a = 0xff,
    };

    const std::filesystem::path font("Bona_Nova_SC/BonaNovaSC-Regular.ttf");
    constexpr const char* message("GAME OVER");
}

#endif // SPECS_MENU_HPP