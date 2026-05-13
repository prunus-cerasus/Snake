#ifndef PLAYER_HPP
#define PLAYER_HPP

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
    PlayerDirection nextDirection;
    PlayerDirection direction;

    Player(size_t p_head = 0, size_t p_body_size = 0)
        : head(p_head), body(p_body_size), nextDirection(PlayerDirection::Up), direction(PlayerDirection::Up) {}
};

#endif // PLAYER_HPP