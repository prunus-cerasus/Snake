#include <cstdint>
#include <variant>

#include "core/SDLState.hpp"
#include "core/Game.hpp"
#include "core/state/GameOver.hpp"

// C++17 Overload pattern
template<class... Ts> struct overloads : Ts... { using Ts::operator()...; };

// Unneeded in C++20
template<class... Ts> overloads(Ts...) -> overloads<Ts...>;

Game::Game() :
    isRunning(true),
    deltaTime(0L),
    currentTime(0L),
    previousTime(0L),
    keyState(SDL_GetKeyboardState(&numkeys)),
    state(state::Empty{})
    {
    if (sdl.valid()) {
        SDL_SetRenderDrawColor(sdl.getRenderer(), 0, 0, 0, 0xff);
        SDL_RenderClear(sdl.getRenderer());
    }
}

Game::~Game() {
    changeState(std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver>{state::Empty{}});
}

void Game::changeState(std::variant<state::Empty, state::Menu, state::Gameplay, state::GameOver> p_newState) {
    state = std::move(p_newState);
}

void Game::updateState() {
    switch (nextState) {
        case state::Type::Empty: {
            break;
        }
        case state::Type::Menu: {
            state = state::Menu(sdl.getTextEngine());
            break;
        }
        case state::Type::Gameplay: {
            state = state::Gameplay();
            break;
        }
        case state::Type::GameOver: {
            state = state::GameOver();
            break;
        }
    }
    nextState = state::Type::Empty;
}

void Game::handleEvents() {
    std::visit(overloads {
        [](state::Empty) {},
        [this](state::Menu& p_menu) { p_menu.handleEvents(this->isRunning, this->keyState, this->nextState); },
        [this](state::Gameplay& p_gameplay) { p_gameplay.handleEvents(this->isRunning, this->keyState, this->nextState); },
        [this](state::GameOver& p_gameOver) { p_gameOver.handleEvents(this->isRunning, this->keyState, this->nextState); },
    }, state);
}

void Game::logic() {
    SDL_Time currentTime;
    SDL_GetCurrentTime(&currentTime);
    if (previousTime == 0) previousTime = currentTime;
    deltaTime = currentTime - previousTime;
    previousTime = currentTime;

    std::visit(overloads {
        [](state::Empty) {},
        [](state::Menu& p_menu) { p_menu.logic(); },
        [this](state::Gameplay& p_gameplay) { p_gameplay.logic(this->deltaTime, this->nextState); },
        [](state::GameOver& p_gameOver) { p_gameOver.logic(); },
    }, state);
}

void Game::render() {

    std::visit(overloads {
        [](state::Empty) {},
        [this](state::Menu& p_menu) { p_menu.render(this->sdl); },
        [this](state::Gameplay& p_gameplay) { p_gameplay.render(this->sdl); },
        [this](state::GameOver& p_gameOver) { p_gameOver.render(this->sdl); },
    }, state);

    SDL_RenderPresent(sdl.getRenderer());
}

void Game::loop() {
    SDL_SetRenderDrawColor(sdl.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(sdl.getRenderer());
    while (isRunning) {
        handleEvents();
        logic();
        updateState();
        render();
    }
}