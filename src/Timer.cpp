#include "Timer.hpp"
#include <algorithm>

Timer::Timer(float ticksPerSecond) : m_ticksPerSecond(ticksPerSecond), m_lastTime(getNanos()), m_passedTime(0.f), m_timeScale(1.f), m_ticks(0), m_partialTicks(0.f) {}

void Timer::advanceTime() {
    auto now = getNanos();
    long long passedNS = now - m_lastTime;
    m_lastTime = now;

    passedNS = std::clamp(passedNS, 0LL, 1000000000LL);

    m_passedTime += passedNS * m_timeScale * m_ticksPerSecond / 1.0e9f;
    m_ticks = std::min(100, static_cast<int>(m_passedTime));
    m_passedTime -= m_ticks;
    m_partialTicks = m_passedTime;
}

int64_t Timer::getNanos() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
}
