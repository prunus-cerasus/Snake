#include <cstdint>
#include <iostream>

#include "core/specs.hpp"
#include "core/vec2.hpp"
#include "core/Entity.hpp"
#include "core/Player.hpp"
#include "core/state/Gameplay.hpp"

state::Gameplay::Gameplay() :
    movePlayerTimer(gamespecs::MOVEMENT_TIME),
    hasConsumed(false),
    mt_gen( std::random_device{}() ),
    distrib_w(0, gamespecs::WIDTH-1),
    distrib_h(0, gamespecs::HEIGHT-1)
    {
    entities.emplace_back(gamespecs::WIDTH/2, gamespecs::HEIGHT/2, EntityType::PlayerHead);
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
    
    if (entities[player.head].pos.x < 0) entities[player.head].pos.x += gamespecs::WIDTH;
    if (entities[player.head].pos.x >= gamespecs::WIDTH) entities[player.head].pos.x -= gamespecs::WIDTH;
    if (entities[player.head].pos.y < 0) entities[player.head].pos.y += gamespecs::HEIGHT;
    if (entities[player.head].pos.y >= gamespecs::HEIGHT) entities[player.head].pos.y -= gamespecs::HEIGHT;
}

void state::Gameplay::drawEntity(SDLState& p_sdl, const Entity& p_obj) {
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

void state::Gameplay::randomizeConsumable(size_t p_consumableIdx) {
    bool foundCollision{false};
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
    } while (foundCollision);
}

void state::Gameplay::generateConsumable(EntityType p_type) {
    const size_t idx{activateEntity(p_type)};
    consumableIds.emplace_back(idx);

    randomizeConsumable(consumableIds.size()-1);
}

void state::Gameplay::removeConsumable(size_t p_consumableIdx) {
    deactivateEntity(consumableIds[p_consumableIdx]);
    consumableIds.erase(consumableIds.begin()+p_consumableIdx);
}

void state::Gameplay::detectCollision(bool& p_isRunning) {
    Entity& playerHead {entities[player.head]};
    for (size_t i{}; i < entities.size(); i++) {
        if (i == player.head) continue;

        Entity& ent {entities[i]};
        if (ent.type == EntityType::Inactive) continue;

        if (playerHead.pos == ent.pos) {
            switch (ent.type) {
                case EntityType::PlayerBody:
                    p_isRunning = false;
                break;

                case EntityType::Apple: {
                    hasConsumed = true;
                    increasePlayer();
                    break;
                }

                case EntityType::Rock: {
                    hasConsumed = true;
                    decreasePlayer();
                    break;
                }

                default:
            }
        }
    }
}

void state::Gameplay::handleEvents(const bool* p_keyState) {
    changePlayerDirection(p_keyState);
}

void state::Gameplay::logic(int64_t p_deltaTime, bool& p_isRunning) {
    if (movePlayerTimer.step(p_deltaTime)) {
        movePlayerTimer.reset();
        movePlayer();
        detectCollision(p_isRunning);

        if (hasConsumed) {
            hasConsumed = false;

            size_t currentRockCount {};
            size_t expectedRockCount {player.body.size() / gamespecs::PLAYERSIZE_PER_ROCK};
            size_t lastRockIndex{};
            for (size_t i{}; i < consumableIds.size(); i++) {
                if (entities[consumableIds[i]].type == EntityType::Rock) {
                    currentRockCount += 1;
                    lastRockIndex = i;
                }
                randomizeConsumable(i);
            }

            if (currentRockCount < expectedRockCount)
                generateConsumable(EntityType::Rock);
            else if (currentRockCount > expectedRockCount)
                removeConsumable(lastRockIndex);
        }
    }
}

void state::Gameplay::render(SDLState& p_sdl) {
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

    for (const auto& entity : entities) {
        if (entity.type == EntityType::Inactive) continue;
        drawEntity(p_sdl, entity);
    }
}