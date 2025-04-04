#pragma once

#include <cstdint>
#include <chrono>

// https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/Random.java

class Random {
private:
    uint64_t seed;
    static const uint64_t multiplier = 0x5DEECE66DLL;
    static const uint64_t addend = 0xBLL;
    static const uint64_t mask = (1LL << 48) - 1;

    int next(int bits) {
        seed = (seed * multiplier + addend) & mask;
        return (int)(seed >> (48 - bits));
    }

    static uint64_t seedUniquifier() {
        static uint64_t curSeedUniquifier = 8682522807148012LL;

        curSeedUniquifier *= 1181783497276652981LL;

        return curSeedUniquifier;
    }

public:
    Random(uint64_t seed) { this->seed = (seed ^ multiplier) & mask; }

    Random() : Random(Random::seedUniquifier() ^ std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) {}

    double nextDouble() { return (((int64_t)(next(26)) << 27) + next(27)) / (double)(1LL << 53); }
};