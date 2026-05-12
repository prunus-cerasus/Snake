#ifndef __SDLSTATE_HPP__
#define __SDLSTATE_HPP__

#include "SDL3/SDL.h"

class SDLState {
public:
    SDLState();
    SDLState(const SDLState&) = delete;
    SDLState(SDLState&&) noexcept;
    ~SDLState();
    bool valid();
    SDL_Window* getWindow() const noexcept;
    SDL_Renderer* getRenderer() const noexcept;
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif // __SDLSTATE_HPP__