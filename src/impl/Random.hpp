#pragma once

#include <cmath>
#include <cstdint>

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

public:
    Random(uint64_t seed) {
        this->seed = (seed ^ multiplier) & mask;
    }

    double nextDouble() {
        return (((int64_t)(next(26)) << 27) + next(27)) / (double)(1LL << 53);
    }
};