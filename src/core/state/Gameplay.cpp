#include <cstdint>
#include <iostream>

#include "core/specs.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/state/Gameplay.hpp"
#include "core/state/Type.hpp"

state::Gameplay::Gameplay() :
    rockCount(0),
    movePlayerTimer(specs::gameplay::MOVEMENT_TIME),
    hasConsumed(false),
    mt_gen( std::random_device{}() ),
    distrib_w(0, specs::gameplay::WIDTH-1),
    distrib_h(0, specs::gameplay::HEIGHT-1)
    {
    entities.emplace_back(specs::gameplay::WIDTH/2, specs::gameplay::HEIGHT/2, EntityType::PlayerHead);
    generateConsumable(EntityType::Apple);
}

void state::Gameplay::changePlayerDirection(const bool* p_keyState) {
    vec2 dir{.x = 0, .y = 0};
    dir.x += p_keyState[SDL_SCANCODE_D];
    dir.x -= p_keyState[SDL_SCANCODE_A];
    dir.y += p_keyState[SDL_SCANCODE_S];
    dir.y -= p_keyState[SDL_SCANCODE_W];

    switch (player.direction) {
        case PlayerDirection::Up:
        case PlayerDirection::Down: {
            if (dir.x == 1) player.nextDirection = PlayerDirection::Right;
            else if (dir.x == -1) player.nextDirection = PlayerDirection::Left;
            break;
        }

        case PlayerDirection::Left:
        case PlayerDirection::Right: {
            if (dir.y == 1) player.nextDirection = PlayerDirection::Down;
            else if (dir.y == -1) player.nextDirection = PlayerDirection::Up;
            break;
        }
    }
}

inline void state::Gameplay::deactivateEntity(size_t p_entityIdx) {
    entities[p_entityIdx].type = EntityType::Inactive;
    inactiveEntityIds.emplace_back(p_entityIdx);
}

inline size_t state::Gameplay::activateEntity(EntityType p_type) {
    Entity ent(-1, -1, p_type);
    if (inactiveEntityIds.size() > 0) {
        size_t idx{inactiveEntityIds.back()};
        inactiveEntityIds.pop_back();
        entities[idx] = ent;
        return idx;
    }

    entities.emplace_back(ent);
    return entities.size()-1;
}

inline void state::Gameplay::increasePlayer() {
    player.body.emplace_back(activateEntity(EntityType::PlayerBody));
}

inline void state::Gameplay::decreasePlayer() {
    deactivateEntity(player.body.back());
    player.body.pop_back();
}

void state::Gameplay::movePlayerBody() {
    if (player.body.size() > 0) {
        auto GetBodyReversed = [&](size_t l_index) -> Entity& {
            return entities[player.body[player.body.size()-l_index-1]];
        };

        for (size_t i{}; i < player.body.size()-1; i++) {
            GetBodyReversed(i).pos = GetBodyReversed(i+1).pos;
        }
        GetBodyReversed(player.body.size()-1).pos = entities[player.head].pos;
    }
}

void state::Gameplay::movePlayerHead() {
    switch (player.direction) {
        case PlayerDirection::Up:
            entities[player.head].pos.y--;
            break;
        case PlayerDirection::Down:
            entities[player.head].pos.y++;
            break;
        case PlayerDirection::Left:
            entities[player.head].pos.x--;
            break;
        case PlayerDirection::Right:
            entities[player.head].pos.x++;
            break;
    }
}

void state::Gameplay::movePlayer() {
    player.direction = player.nextDirection;

    movePlayerBody();
    movePlayerHead();
    
    if (entities[player.head].pos.x < 0) entities[player.head].pos.x += specs::gameplay::WIDTH;
    if (entities[player.head].pos.x >= specs::gameplay::WIDTH) entities[player.head].pos.x -= specs::gameplay::WIDTH;
    if (entities[player.head].pos.y < 0) entities[player.head].pos.y += specs::gameplay::HEIGHT;
    if (entities[player.head].pos.y >= specs::gameplay::HEIGHT) entities[player.head].pos.y -= specs::gameplay::HEIGHT;
}

void state::Gameplay::drawEntity(SDLState& p_sdl, const Entity& p_obj) {
    SDL_Color dcolor{specs::gameplay::DRAWCOLOR[static_cast<size_t>(p_obj.type)]};

    SDL_SetRenderDrawColor(p_sdl.getRenderer(), dcolor.r, dcolor.g, dcolor.b, dcolor.a);

    SDL_FRect dest{
        .x = static_cast<float>(specs::gameplay::CELL_SIDE*p_obj.pos.x+1),
        .y = static_cast<float>(specs::gameplay::CELL_SIDE*p_obj.pos.y+1),
        .w = static_cast<float>(specs::gameplay::CELL_SIDE-1),
        .h = static_cast<float>(specs::gameplay::CELL_SIDE-1),
    };
    SDL_RenderFillRect(p_sdl.getRenderer(), &dest);
}

void state::Gameplay::randomizeConsumable(size_t p_consumableIdx) {
    bool foundCollision{false};
    int32_t j{};
    do {
        Entity& ent = entities[consumableIds[p_consumableIdx]];
        ent.pos.x = distrib_w(mt_gen);
        ent.pos.y = distrib_h(mt_gen);

        foundCollision = false;
        for (size_t i{}; i < p_consumableIdx; i++) {
            if (entities[consumableIds[i]].pos == ent.pos) {
                foundCollision = true;
                continue;
            }
        }

        if (entities[player.head].pos == ent.pos)
            foundCollision = true;

    } while (foundCollision && ++j < specs::gameplay::MAX_RANDOM_LOOP_SIZE);

    if (foundCollision) {
        // TODO: If cannot find an empty spot, change state to GameOver
    }
}

void state::Gameplay::generateConsumable(EntityType p_type) {
    const size_t idx{activateEntity(p_type)};
    consumableIds.emplace_back(idx);

    randomizeConsumable(consumableIds.size()-1);
}

void state::Gameplay::removeConsumable(size_t p_consumableIdx) {
    deactivateEntity(consumableIds[p_consumableIdx]);
    std::swap(consumableIds[p_consumableIdx], consumableIds.back());
    consumableIds.pop_back();
}

void state::Gameplay::detectCollision(state::Type& p_nextState) {
    Entity& playerHead {entities[player.head]};
    for (size_t i{}; i < entities.size(); i++) {
        if (i == player.head) continue;

        Entity& ent {entities[i]};
        if (ent.type == EntityType::Inactive) continue;

        if (playerHead.pos == ent.pos) {
            switch (ent.type) {
                case EntityType::PlayerBody:
                    p_nextState = Type::GameOver;
                break;

                case EntityType::Apple: {
                    hasConsumed = true;
                    increasePlayer();
                    break;
                }

                case EntityType::Rock: {
                    hasConsumed = true;
                    for (int32_t i{}; i < specs::gameplay::PLAYERSIZE_PER_ROCK; i++) decreasePlayer();
                    break;
                }

                default:
            }
        }
    }
}

void state::Gameplay::handleEvents(bool& p_isRunning, const bool* p_keyState, state::Type& p_nextState) {
    bool is_escapeDown{p_keyState[SDL_SCANCODE_ESCAPE]};

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) p_isRunning = false;
        else if (ev.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_ESCAPE && !is_escapeDown) {
                p_nextState = state::Type::Menu;
            }
        }
    }

    changePlayerDirection(p_keyState);
}

void state::Gameplay::logic(int64_t p_deltaTime, state::Type& p_nextState) {
    if (movePlayerTimer.step(p_deltaTime)) {
        movePlayerTimer.reset();
        movePlayer();
        detectCollision(p_nextState);

        if (hasConsumed) {
            hasConsumed = false;
            size_t expectedRockCount{player.body.size()/state::specs::gameplay::PLAYERSIZE_PER_ROCK};
            for (size_t i{consumableIds.size()}; i > 0; i--) {
                randomizeConsumable(i-1);
                if (entities[consumableIds[i-1]].type == EntityType::Rock) {
                    if (rockCount > expectedRockCount) {
                        removeConsumable(i-1);
                        rockCount--;
                    }
                }
            }

            for (; rockCount < expectedRockCount; rockCount++)
                generateConsumable(EntityType::Rock);
        }
    }
}

void state::Gameplay::render(SDLState& p_sdl) {
    SDL_SetRenderDrawColor(
        p_sdl.getRenderer(),
        specs::gameplay::BG_COLOR.r,
        specs::gameplay::BG_COLOR.g,
        specs::gameplay::BG_COLOR.b,
        specs::gameplay::BG_COLOR.a
    );

    const SDL_FRect bg_dest {
        .x = 0.0f,
        .y = 0.0f,
        .w = static_cast<float>(::specs::LOGICAL_WIDTH),
        .h = static_cast<float>(::specs::LOGICAL_HEIGHT),
    };

    SDL_RenderFillRect(p_sdl.getRenderer(), &bg_dest);

    for (const auto& entity : entities) {
        if (entity.type == EntityType::Inactive) continue;
        drawEntity(p_sdl, entity);
    }
}