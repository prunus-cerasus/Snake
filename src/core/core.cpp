#include <iostream>
#include <random>

#include "SDL3/SDL.h"

#include "core/SDLState.hpp"
#include "core/Timer.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/Game.hpp"

#include "core/specs.hpp"

void loop(Game& p_game) {
    p_game.changeState(state::Gameplay());
    std::get<state::Gameplay>(p_game.state).generateConsumable(EntityType::Apple);

    while (p_game.isRunning) {
        p_game.handleEvents();
        p_game.logic();
        p_game.render();
    }
}

void core() {
    Game game;
    if (!game.sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return;
    }
    loop(game);
}