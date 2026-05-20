#ifndef MENU_HPP
#define MENU_HPP

#include <variant>

#include "core/SDLState.hpp"
#include "core/state/Type.hpp"

namespace state {
    struct Menu {

        Menu(TTF_TextEngine* p_textEngine);
        Menu(const Menu&) = delete;
        Menu& operator=(const Menu&) = delete;
        Menu(Menu&& p_other) noexcept;
        Menu& operator=(Menu&& p_other) noexcept;
        ~Menu();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
        void logic();
        void render(SDLState& p_sdl);

        bool isFilled;
        TTF_Font* font;
        TTF_Text* text;
    };
}

#endif // MENU_HPP