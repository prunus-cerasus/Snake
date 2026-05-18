#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <cstdint>

#include "core/SDLState.hpp"
#include "core/state/Type.hpp"

namespace state { 
    struct GameOver {

        GameOver();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
        void logic();
        void render(SDLState& p_sdl);

        int32_t isFilled;
    };
}

#endif // GAMEOVER_HPP