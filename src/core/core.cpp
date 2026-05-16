#include <iostream>

#include "core/Game.hpp"

void core() {
    Game game;
    if (!game.sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return;
    }

    game.changeState(state::Gameplay());
    game.loop();
}