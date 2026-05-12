#include "core/Timer.hpp"

Timer::Timer(int64_t p_interval) : timer(0L), interval(p_interval), isDone(false) {}

void Timer::reset() {
    timer -= interval;
    isDone = false;
}

bool Timer::step(int64_t p_deltaTime) {
    timer += p_deltaTime;
    if (timer >= interval) {
        isDone = true;
    }
    return isDone;
}