#ifndef TIMER_H_
#define TIMER_H_

#include <GyverDS3231.h>

namespace my {

class Timer {
public:
public:
    Timer() = default;

    void Setup();
    void LogCurrentTime();
    Datime GetTime();
    bool Tick();
};

} // namespace

#endif // TIMER_H_
