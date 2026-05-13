#ifndef TIMER_HPP
#define TIMER_HPP

#include <cstdint>

class Timer {
public:
    Timer(int64_t p_interval);
    void reset();
    bool step(int64_t p_deltaTime);
private:
    int64_t timer;
    int64_t interval;
    bool isDone;
};

#endif // TIMER_HPP