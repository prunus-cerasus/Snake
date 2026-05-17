#ifndef SPECS_HPP
#define SPECS_HPP

#include "SDL3/SDL.h"

namespace specs {
    constexpr int LOGICAL_WIDTH {800};
    constexpr int LOGICAL_HEIGHT {600};
    constexpr const char* TITLE {"Program"};
    constexpr int FLAGS {SDL_WINDOW_RESIZABLE};
    constexpr int MAX_BUFFER_COUNT {4};
}

#endif // SPECS_HPP