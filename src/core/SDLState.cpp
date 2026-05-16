#include "core/SDLState.hpp"

#include "core/specs.hpp"

SDLState::SDLState() : m_window(nullptr), m_renderer(nullptr) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) return;

    m_window = SDL_CreateWindow(specs::TITLE, specs::LOGICAL_WIDTH, specs::LOGICAL_HEIGHT, specs::FLAGS);
    if (m_window == nullptr) return;

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (m_renderer == nullptr) return;

    SDL_SetRenderLogicalPresentation(
        m_renderer,
        specs::LOGICAL_WIDTH,
        specs::LOGICAL_HEIGHT,
        SDL_LOGICAL_PRESENTATION_LETTERBOX
    );

    SDL_SetRenderVSync(m_renderer, 1);
}

SDLState::SDLState(SDLState&& p_other) noexcept : m_window(p_other.m_window), m_renderer(p_other.m_renderer) {
    p_other.m_window = nullptr;
    p_other.m_renderer = nullptr;
}

SDLState::~SDLState() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool SDLState::valid() {
    return m_window != nullptr && m_renderer != nullptr;
}

SDL_Window* SDLState::getWindow() const noexcept {
    return m_window;
}

SDL_Renderer* SDLState::getRenderer() const noexcept {
    return m_renderer;
}