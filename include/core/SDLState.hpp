#ifndef SDLSTATE_HPP
#define SDLSTATE_HPP

#include "SDL3/SDL.h"

class SDLState {
public:
    SDLState();
    SDLState(const SDLState&) = delete;
    SDLState& operator=(const SDLState&) = delete;
    SDLState& operator=(SDLState&&) = delete;
    SDLState(SDLState&&) noexcept;
    ~SDLState();
    bool valid();
    SDL_Window* getWindow() const noexcept;
    SDL_Renderer* getRenderer() const noexcept;
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    bool m_isValid;
};

#endif // SDLSTATE_HPP