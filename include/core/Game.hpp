#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <variant>

#include "core/SDLState.hpp"
#include "core/state/Empty.hpp"
#include "core/state/Menu.hpp"
#include "core/state/Gameplay.hpp"
#include "core/state/GameOver.hpp"

struct Game {
    Game();
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;
    ~Game();

    void changeState(std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver> p_newState);

    void handleEvents();
    void logic();
    void render();

    void loop();

    SDLState sdl;

    bool isRunning;
    int64_t deltaTime;
    int64_t currentTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver> state;
};

#endif // GAME_HPP