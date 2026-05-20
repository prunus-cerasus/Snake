#ifndef SDLSTATE_HPP
#define SDLSTATE_HPP

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

class SDLState {
public:
    SDLState();
    SDLState(const SDLState&) = delete;
    SDLState& operator=(const SDLState&) = delete;
    SDLState& operator=(SDLState&&) = delete;
    SDLState(SDLState&&) noexcept;
    ~SDLState();
    bool valid() const noexcept;
    SDL_Window* getWindow() const noexcept;
    SDL_Renderer* getRenderer() const noexcept;
    TTF_TextEngine* getTextEngine() const noexcept;
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    TTF_TextEngine* m_textEngine;

    bool m_isValid;
};

#endif // SDLSTATE_HPP