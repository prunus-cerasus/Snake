#include "core/SDLState.hpp"
#include "core/specs.hpp"
#include "core/state/Menu.hpp"
#include "core/state/Type.hpp"
#include "core/state/specs/menu.hpp"

#include <iostream>

state::Menu::Menu(const std::filesystem::path& p_assets_path, TTF_TextEngine* p_textEngine) : isFilled(false), font(nullptr), text(nullptr) {
    font = TTF_OpenFont((p_assets_path / specs::menu::font).string().c_str(), 40.0f);
    if (font == nullptr) {
        std::cerr << "SDL_ttf Error: " << SDL_GetError() << '\n';
        return;
    }

    text = TTF_CreateText(p_textEngine, font, specs::menu::message, 0);
    if (text == nullptr) {
        std::cerr << "SDL_ttf Error: " << SDL_GetError() << '\n';
        return;
    }

    TTF_SetTextColor(
        text,
        specs::menu::TEXT_COLOR.r,
        specs::menu::TEXT_COLOR.g,
        specs::menu::TEXT_COLOR.b,
        specs::menu::TEXT_COLOR.a
    );
}

state::Menu::Menu(Menu&& p_other) noexcept : isFilled(p_other.isFilled), font(p_other.font), text(p_other.text) {
    p_other.font = nullptr;
    p_other.text = nullptr;
}

state::Menu& state::Menu::operator=(Menu&& p_other) noexcept {
    if (this == &p_other) return *this;

    if (this->text != nullptr) TTF_DestroyText(this->text);
    if (this->font != nullptr) TTF_CloseFont(this->font);

    this->font = p_other.font;
    this->text = p_other.text;
    this->isFilled = p_other.isFilled;
    p_other.font = nullptr;
    p_other.text = nullptr;

    return *this;
}

state::Menu::~Menu() {
    if (text != nullptr) TTF_DestroyText(text);
    if (font != nullptr) TTF_CloseFont(font);
}

void state::Menu::handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState) {
    bool is_spaceDown = p_keyState[SDL_SCANCODE_SPACE];

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_isRunning = false;
        else if (ev.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_SPACE && !is_spaceDown) {
                p_nextState = state::Type::Gameplay;
            }
        }
    }
}

void state::Menu::logic() {

}

void state::Menu::render(SDLState& p_sdl) {
    isFilled = true;
    SDL_SetRenderDrawColor(
        p_sdl.getRenderer(),
        specs::menu::BG_COLOR.r,
        specs::menu::BG_COLOR.g,
        specs::menu::BG_COLOR.b,
        specs::menu::BG_COLOR.a
    );
    SDL_FRect dest{
        .x = 0,
        .y = 0,
        .w = ::specs::LOGICAL_WIDTH,
        .h = ::specs::LOGICAL_HEIGHT,
    };

    SDL_RenderFillRect(p_sdl.getRenderer(), &dest);

    if (text != nullptr) {
        int32_t text_width{};
        int32_t text_height{};
        TTF_GetTextSize(text, &text_width, &text_height);

        TTF_DrawRendererText(text, static_cast<float>(
            ::specs::LOGICAL_WIDTH - text_width)*0.5f,
            static_cast<float>(::specs::LOGICAL_HEIGHT - text_height)*0.5f
        );
    }
}
