#include <iostream>
#include <cstdint>
#include <span>

#include "core/Game.hpp"

int main() {
    Game game;
    if (!game.sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return 1;
    }

    game.changeState(state::Menu(game.sdl.getTextEngine()));
    game.loop();
    return 0;
}