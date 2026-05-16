#include <iostream>

#include "core/Game.hpp"

int main() {
    Game game;
    if (!game.sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return 1;
    }

    game.changeState(state::Gameplay());
    game.loop();
    return 0;
}