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
    SDL_Window* get_window() const noexcept;
    SDL_Renderer* get_renderer() const noexcept;
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};

#endif // __SDLSTATE_HPP__