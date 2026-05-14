#include <iostream>
#include <random>

#include "SDL3/SDL.h"

#include "core/SDLState.hpp"
#include "core/Timer.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"

#include "core/specs.hpp"

struct GameState {
    bool isRunning;
    int64_t deltaTime;
    int64_t previousTime;

    const bool* keyState;
    int32_t numkeys;

    std::vector<Entity> entities;
    std::vector<size_t> inactiveEntityIds;
    std::vector<size_t> consumableIds;

    Player player;
    Timer movePlayerTimer;
    bool hasConsumed;


    std::random_device rd;
    std::mt19937 mt_gen;
    std::uniform_int_distribution<> distrib_w;
    std::uniform_int_distribution<> distrib_h;

    GameState()
        : isRunning(true),
        deltaTime(0L),
        previousTime(0L),
        keyState(SDL_GetKeyboardState(&numkeys)),
        movePlayerTimer(gamespecs::MOVEMENT_TIME),
        hasConsumed(false),
        mt_gen(rd()),
        distrib_w(0, gamespecs::WIDTH-1),
        distrib_h(0, gamespecs::HEIGHT-1)
        {
        entities.emplace_back(0, 0, EntityType::PlayerHead);
    }
};

void changePlayerDirection(GameState& p_game) {
    vec2 dir{.x = 0, .y = 0};
    dir.x += p_game.keyState[SDL_SCANCODE_D];
    dir.x -= p_game.keyState[SDL_SCANCODE_A];
    dir.y += p_game.keyState[SDL_SCANCODE_S];
    dir.y -= p_game.keyState[SDL_SCANCODE_W];

    switch (p_game.player.direction) {
        case PlayerDirection::Up:
        case PlayerDirection::Down: {
            if (dir.x == 1) p_game.player.nextDirection = PlayerDirection::Right;
            else if (dir.x == -1) p_game.player.nextDirection = PlayerDirection::Left;
            break;
        }

        case PlayerDirection::Left:
        case PlayerDirection::Right: {
            if (dir.y == 1) p_game.player.nextDirection = PlayerDirection::Down;
            else if (dir.y == -1) p_game.player.nextDirection = PlayerDirection::Up;
            break;
        }
    }

}

inline void deactivateEntity(GameState& p_game, size_t p_entityIdx) {
    p_game.entities[p_entityIdx].type = EntityType::Inactive;
    p_game.inactiveEntityIds.emplace_back(p_entityIdx);
}

inline size_t activateEntity(GameState& p_game, EntityType p_type) {
    Entity ent(-1, -1, p_type);
    if (p_game.inactiveEntityIds.size() > 0) {
        size_t idx{p_game.inactiveEntityIds.back()};
        p_game.inactiveEntityIds.pop_back();
        p_game.entities[idx] = ent;
        return idx;
    }

    p_game.entities.emplace_back(ent);
    return p_game.entities.size()-1;
}

inline void increasePlayer(GameState& p_game) {
    p_game.player.body.emplace_back(activateEntity(p_game, EntityType::PlayerBody));
}

inline void decreasePlayer(GameState& p_game) {
    deactivateEntity(p_game, p_game.player.body.back());
    p_game.player.body.pop_back();
}

void movePlayerBody(GameState& p_game) {
    if (p_game.player.body.size() > 0) {
        auto GetBodyReversed = [&](size_t l_index) -> Entity& {
            return p_game.entities[p_game.player.body[p_game.player.body.size()-l_index-1]];
        };

        for (size_t i{}; i < p_game.player.body.size()-1; i++) {
            GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
        }
        GetBodyReversed(p_game.player.body.size()-1).pos = p_game.entities[p_game.player.head].pos;
    }
}

void movePlayerHead(GameState& p_game) {
    switch (p_game.player.direction) {
        case PlayerDirection::Up:
            p_game.entities[p_game.player.head].pos.y--;
            break;
        case PlayerDirection::Down:
            p_game.entities[p_game.player.head].pos.y++;
            break;
        case PlayerDirection::Left:
            p_game.entities[p_game.player.head].pos.x--;
            break;
        case PlayerDirection::Right:
            p_game.entities[p_game.player.head].pos.x++;
            break;
    }
}

void movePlayer(GameState& p_game) {
    p_game.player.direction = p_game.player.nextDirection;

    movePlayerBody(p_game);
    movePlayerHead(p_game);
    
    if (
        p_game.entities[p_game.player.head].pos.x < 0 ||
        p_game.entities[p_game.player.head].pos.x >= gamespecs::WIDTH ||
        p_game.entities[p_game.player.head].pos.y < 0 ||
        p_game.entities[p_game.player.head].pos.y >= gamespecs::HEIGHT
    ) p_game.isRunning = false;
}

void drawEntity(SDLState& p_sdl, const Entity& p_obj) {
    SDL_Color dcolor{gamespecs::DRAWCOLOR[static_cast<size_t>(p_obj.type)]};

    SDL_SetRenderDrawColor(p_sdl.getRenderer(), dcolor.r, dcolor.g, dcolor.b, dcolor.a);

    SDL_FRect dest{
        .x = static_cast<float>(gamespecs::CELL_SIDE*p_obj.pos.x+1),
        .y = static_cast<float>(gamespecs::CELL_SIDE*p_obj.pos.y+1),
        .w = static_cast<float>(gamespecs::CELL_SIDE-1),
        .h = static_cast<float>(gamespecs::CELL_SIDE-1),
    };
    SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
}

void handleEvents([[maybe_unused]] SDLState& p_sdl, GameState& p_game) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_game.isRunning = false;
    }

    changePlayerDirection(p_game);
}

void randomizeConsumable(GameState& p_game, size_t p_consumableIdx) {
    bool foundCollision{false};
    do {
        Entity& ent = p_game.entities[p_game.consumableIds[p_consumableIdx]];
        ent.pos.x = p_game.distrib_w(p_game.mt_gen);
        ent.pos.y = p_game.distrib_h(p_game.mt_gen);

        foundCollision = false;
        for (size_t i{}; i < p_consumableIdx; i++) {
            if (p_game.entities[p_game.consumableIds[i]].pos == ent.pos) {
                foundCollision = true;
                continue;
            }
        }

        if (p_game.entities[p_game.player.head].pos == ent.pos)
            foundCollision = true;
    } while (foundCollision);
}

void generateConsumable(GameState& p_game, EntityType p_type) {
    const size_t idx{activateEntity(p_game, p_type)};
    p_game.consumableIds.emplace_back(idx);

    randomizeConsumable(p_game, p_game.consumableIds.size()-1);
}

void removeConsumable(GameState& p_game, size_t p_consumableIdx) {
    deactivateEntity(p_game, p_game.consumableIds[p_consumableIdx]);
    p_game.consumableIds.erase(p_game.consumableIds.begin()+p_consumableIdx);
}

void detectCollision(GameState& p_game) {
    Entity& playerHead {p_game.entities[p_game.player.head]};
    for (size_t i{}; i < p_game.entities.size(); i++) {
        if (i == p_game.player.head) continue;

        Entity& ent {p_game.entities[i]};
        if (ent.type == EntityType::Inactive) continue;

        if (playerHead.pos == ent.pos) {
            switch (ent.type) {
                case EntityType::PlayerBody:
                    p_game.isRunning = false;
                break;

                case EntityType::Apple: {
                    p_game.hasConsumed = true;
                    increasePlayer(p_game);
                    break;
                }

                case EntityType::Rock: {
                    p_game.hasConsumed = true;
                    decreasePlayer(p_game);
                    break;
                }

                default:
            }
        }
    }
}

void logic([[maybe_unused]] SDLState& p_sdl, GameState& p_game) {
    SDL_Time currentTime;
    SDL_GetCurrentTime(&currentTime);
    if (p_game.previousTime == 0) p_game.previousTime = currentTime;
    p_game.deltaTime = currentTime - p_game.previousTime;
    p_game.previousTime = currentTime;

    if (p_game.movePlayerTimer.step(p_game.deltaTime)) {
        p_game.movePlayerTimer.reset();
        movePlayer(p_game);
        detectCollision(p_game);
        
        if (p_game.hasConsumed) {
            p_game.hasConsumed = false;

            size_t currentRockCount {};
            size_t expectedRockCount {p_game.player.body.size() / gamespecs::PLAYERSIZE_PER_ROCK};
            size_t lastRockIndex{};
            for (size_t i{}; i < p_game.consumableIds.size(); i++) {
                if (p_game.entities[p_game.consumableIds[i]].type == EntityType::Rock) {
                    currentRockCount += 1;
                    lastRockIndex = i;
                }
                randomizeConsumable(p_game, i);
            }

            if (currentRockCount < expectedRockCount)
                generateConsumable(p_game, EntityType::Rock);
            else if (currentRockCount > expectedRockCount)
                removeConsumable(p_game, lastRockIndex);
        }
    }
}

void render(SDLState& p_sdl, GameState& p_game) {
    SDL_SetRenderDrawColor(p_sdl.getRenderer(), 0, 0, 0, 0xff);
    SDL_RenderClear(p_sdl.getRenderer());
    SDL_SetRenderDrawColor(
        p_sdl.getRenderer(),
        gamespecs::BG_COLOR.r,
        gamespecs::BG_COLOR.g,
        gamespecs::BG_COLOR.b,
        gamespecs::BG_COLOR.a
    );

    const SDL_FRect bg_dest {
        .x = 0.0f,
        .y = 0.0f,
        .w = static_cast<float>(specs::LOGICAL_WIDTH),
        .h = static_cast<float>(specs::LOGICAL_HEIGHT),
    };

    SDL_RenderFillRect(p_sdl.getRenderer(), &bg_dest);

    for (const auto& entity : p_game.entities) {
        if (entity.type == EntityType::Inactive) continue;
        drawEntity(p_sdl, entity);
    }

    SDL_RenderPresent(p_sdl.getRenderer());
}

void loop(SDLState& p_sdl) {
    GameState game;
    game.entities[game.player.head].pos = {.x = gamespecs::WIDTH/2, .y = gamespecs::HEIGHT/2};
    generateConsumable(game, EntityType::Apple);
    // for (int32_t i = 0; i < 10; i++) game.increasePlayer();

    while (game.isRunning) {
        handleEvents(p_sdl, game);
        logic(p_sdl, game);
        render(p_sdl, game);
    }
}

void core() {
    SDLState sdl;
    if (!sdl.valid()) {
        std::cerr << "Fatal Error:"  << SDL_GetError() << '\n';
        return;
    }
    loop(sdl);
}