#include <SDL3/SDL_main.h>

#include <iostream>
#include <cstdint>
#include <vector>
#include <string_view>

#include "core/Game.hpp"

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args(argv, argv + argc);

    Game game(args);
    if (!game.sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return 1;
    }

    game.loop();
    return 0;
}
