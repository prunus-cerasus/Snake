#include "core/SDLState.hpp"

namespace specs {
    constexpr int WIDTH {800};
    constexpr int HEIGHT {600};
    constexpr const char* TITLE {"Program"};
    constexpr int FLAGS {SDL_WINDOW_RESIZABLE};
}

SDLState::SDLState() : m_window(nullptr), m_renderer(nullptr) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) return;

    m_window = SDL_CreateWindow(specs::TITLE, specs::WIDTH, specs::HEIGHT, specs::FLAGS);
    if (m_window == nullptr) return;

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (m_renderer == nullptr) return;
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

SDL_Window* SDLState::get_window() const noexcept {
    return m_window;
}

SDL_Renderer* SDLState::get_renderer() const noexcept {
    return m_renderer;
}