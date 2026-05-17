#ifndef MENU_HPP
#define MENU_HPP

#include <variant>

#include "core/SDLState.hpp"
#include "core/state/Type.hpp"

namespace state {
    struct Menu {

        Menu();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
        void logic();
        void render(SDLState& p_sdl);

        bool isFilled;
    };
}

#endif // MENU_HPP