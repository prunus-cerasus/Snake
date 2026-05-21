#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <cstdint>

#include "core/SDLState.hpp"
#include "core/state/Type.hpp"

namespace state { 
    struct GameOver {

        GameOver(const std::filesystem::path& p_assets_path, TTF_TextEngine* p_textEngine);
        GameOver(const GameOver&) = delete;
        GameOver& operator=(const GameOver&) = delete;
        GameOver(GameOver&& p_other) noexcept;
        GameOver& operator=(GameOver&& p_other) noexcept;
        ~GameOver();

        void handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState);
        void logic();
        void render(SDLState& p_sdl);

        bool isFilled;
        TTF_Font* font;
        TTF_Text* text;
    };
}

#endif // GAMEOVER_HPP