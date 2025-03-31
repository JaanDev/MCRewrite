#pragma once
#include <cstdint>

enum class BlockID : uint8_t {
    AIR = 0,
    ROCK,
    GRASS,
};

struct UV {
    float startX;
    float startY;
    float endX;
    float endY;
};

struct Vertex {
    float x, y, z;
    float r, g, b;
    float u, v;
};

enum class Faces {
    Up,    // y+
    Down,  // y-
    Back,  // z+
    Front, // z-
    Left,  // x+
    Right  // x-
};

#define DO_TIME_MEASURING

#ifdef DO_TIME_MEASURING
#define TIME_MEASURE_BEGIN(name) auto name##_begin = std::chrono::system_clock::now();
#define TIME_MEASURE_END(name) auto name##_end = std::chrono::system_clock::now();
#define TIME_MEASURE_DBG(name)                                                                                                             \
    logD("Time measure for " #name ": {} millis",                                                                                          \
         std::chrono::duration_cast<std::chrono::microseconds>(name##_end - name##_begin).count() / 1000.f);
#else
#define TIME_MEASURE_BEGIN(name)
#define TIME_MEASURE_END(name)
#define TIME_MEASURE_DBG(name)
#endif