#ifndef VEC2_HPP
#define VEC2_HPP

struct vec2 {
    int x;
    int y;

    bool operator==(const vec2 p_other) const {
        return x == p_other.x && y == p_other.y;
    }
};

#endif // VEC2_HPP