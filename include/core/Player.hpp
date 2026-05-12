#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <cstdint>
#include <vector>

enum class PlayerDirection {
    Up,
    Left,
    Down,
    Right,
};

struct Player {
    size_t head;
    std::vector<size_t> body;
    PlayerDirection direction;

    Player(size_t p_head = 0, size_t p_body_size = 0) : head(p_head), body(p_body_size), direction(PlayerDirection::Up) {}
};

#endif // __PLAYER_HPP__