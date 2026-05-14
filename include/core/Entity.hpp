#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/SDLState.hpp"
#include "core/vec2.hpp"

enum class EntityType {
    PlayerHead,
    PlayerBody,
    Apple,
    Rock,
    Inactive
};

struct Entity {
    vec2 pos;
    EntityType type;

    Entity(int p_x = 0, int p_y = 0, EntityType p_type = EntityType::PlayerHead) : pos({p_x, p_y}), type(p_type) {}
};

#endif // ENTITY_HPP