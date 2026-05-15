#include <cstdint>
#include <variant>

#include "SDL3/SDL.h"

#include "core/Game.hpp"

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
}

Game::~Game() {
    changeState(std::variant<state::Empty, state::Menu*, state::Gameplay*, state::GameOver*>{state::Empty{}});
}

void Game::changeState(std::variant<state::Empty, state::Menu*, state::Gameplay*, state::GameOver*> p_newState) {
    std::visit(overloads {
        [](state::Empty) {},
        [](state::Menu* p_menu) { delete p_menu; },
        [](state::Gameplay* p_gameplay) { delete p_gameplay; },
        [](state::GameOver* p_gameOver) { delete p_gameOver; },
    }, state);

    state = std::move(p_newState);
}

void Game::handleEvents() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) isRunning = false;
    }

    std::visit(overloads {
        [](state::Empty) {},
        [this](state::Menu* p_menu) { p_menu->handleEvents(this); },
        [this](state::Gameplay* p_gameplay) { p_gameplay->handleEvents(this); },
        [this](state::GameOver* p_gameOver) { p_gameOver->handleEvents(this); },
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
        [this](state::Menu* p_menu) { p_menu->logic(this); },
        [this](state::Gameplay* p_gameplay) { p_gameplay->logic(this); },
        [this](state::GameOver* p_gameOver) { p_gameOver->logic(this); },
    }, state);
}

void Game::render() {
    SDL_SetRenderDrawColor(sdl.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(sdl.getRenderer());

    std::visit(overloads {
        [](state::Empty) {},
        [this](state::Menu* p_menu) { p_menu->render(this); },
        [this](state::Gameplay* p_gameplay) { p_gameplay->render(this); },
        [this](state::GameOver* p_gameOver) { p_gameOver->render(this); },
    }, state);

    SDL_RenderPresent(sdl.getRenderer());
}