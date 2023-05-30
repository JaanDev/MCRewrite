#pragma once
#include "includes.hpp"
#include <chrono>

class Timer {
  public:
    Timer(float ticksPerSecond);
    void advanceTime();
    float getA();
    uint32_t getTicks();

  private:
    int64_t getNanos();

    std::chrono::high_resolution_clock m_clock;
    float m_ticksPerSecond;
    int64_t m_lastTime;
    float m_passedTime;
    float m_timeScale;
    uint32_t m_ticks;
    float m_a; // i have tried to understand what this is... i failed
};