#pragma once
#include <chrono>

class Timer {
public:
    Timer(float ticksPerSecond);
    void advanceTime();
    inline uint32_t getTicks() const { return m_ticks; }
    inline float getPartialTicks() const { return m_partialTicks; }

private:
    int64_t getNanos();

    std::chrono::high_resolution_clock m_clock;
    float m_ticksPerSecond;
    int64_t m_lastTime;
    float m_passedTime;
    float m_timeScale;
    uint32_t m_ticks;
    float m_partialTicks;
};