#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "core/SDLState.hpp"
#include "core/vec2.hpp"

enum class EntityType {
    PlayerHead,
    PlayerBody,
    Apple,
    Stone,
};

struct Entity {
    vec2 pos;
    EntityType type;

    Entity(int p_x = 0, int p_y = 0, EntityType p_type = EntityType::PlayerHead) : pos({p_x, p_y}), type(p_type) {}
};

#endif // __ENTITY_HPP__