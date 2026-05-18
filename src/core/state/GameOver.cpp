#include "core/specs.hpp"
#include "core/state/GameOver.hpp"

state::GameOver::GameOver() : isFilled(0) {};

void state::GameOver::handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState) {
    bool is_escapeDown{p_keyState[SDL_SCANCODE_ESCAPE]};

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_isRunning = false;
        else if (ev.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_ESCAPE && !is_escapeDown) {
                p_nextState = state::Type::Menu;
            }
        }
    }
}

void state::GameOver::logic() {

}

void state::GameOver::render(SDLState& p_sdl) {
    if (!isFilled) {
        isFilled = true;
        // SDL_SetRenderDrawBlendMode(p_sdl.getRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(p_sdl.getRenderer(), 0x40, 0x40, 0xa0, 0xff);
        // SDL_RenderClear(p_sdl.getRenderer());
        SDL_FRect dest{
            .x = 0,
            .y = 0,
            .w = specs::LOGICAL_WIDTH,
            .h = specs::LOGICAL_HEIGHT,
        };

        SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
        SDL_RenderPresent(p_sdl.getRenderer());
        SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
        SDL_RenderPresent(p_sdl.getRenderer());
        SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
        SDL_RenderPresent(p_sdl.getRenderer());
        SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
        SDL_RenderPresent(p_sdl.getRenderer());

        // SDL_SetRenderDrawBlendMode(p_sdl.getRenderer(), SDL_BLENDMODE_NONE);
    }
}