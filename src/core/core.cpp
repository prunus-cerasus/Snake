#include <iostream>
#include "SDL3/SDL.h"
#include "core/SDLState.hpp"

void loop(SDLState& p_state) {
    bool is_running{true};
    while (is_running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) is_running = false;
        }

        SDL_RenderClear(p_state.get_renderer());
        SDL_RenderPresent(p_state.get_renderer());
    }
}

void core() {
    SDLState state;
    if (!state.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return;
    }
    loop(state);
}