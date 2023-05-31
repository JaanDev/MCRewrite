#pragma once
#include "includes.hpp"
#include "rlgl.h"

const int chunkSize = 16;    // x and z
const int chunkHeight = 256; // y
const int chunksCount = 16;

const int surfaceLevel = 170; // surface level

const float mouseSensitivity = .003f;

enum class BlockTypes : uint8_t {
    Air = 0,
    Rock,
    Grass,
};

struct ChunkPos {
    int32_t x;
    int32_t z;

    bool operator==(const ChunkPos& other) const {
        return x == other.x && z == other.z;
    }

    ChunkPos operator+(const ChunkPos& other) const {
        return {x + other.x, z + other.z};
    }
};

struct BlockPos {
    int32_t x;
    int32_t y;
    int32_t z;

    BlockPos operator+(const BlockPos& other) {
        return {x + other.x, y + other.y, z + other.z};
    }

    BlockPos operator-(const BlockPos& other) {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator+(Vector3 other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    // global (level) pos to local (chunk) pos
    BlockPos local() const {
        auto x2 = x % chunkSize;
        if (x2 < 0)
            x2 = chunkSize - x2;

        auto z2 = z % chunkSize;
        if (z2 < 0)
            z2 = chunkSize - z2;

        return {x2, y, z2};
    }

    bool isInChunk() const {
        return x >= 0 && x < chunkSize && y >= 0 && y < chunkHeight && z >= 0 && z < chunkSize;
    }

    ChunkPos chunkPos() const {
        return ChunkPos {static_cast<int32_t>(floorf(x / (float)chunkSize)), static_cast<int32_t>(floorf(z / (float)chunkSize))};
    }
};

template <>
struct std::hash<ChunkPos> {
    size_t operator()(const ChunkPos& p) const {
        using std::hash;

        return hash<int32_t>()(p.x) ^ (hash<int32_t>()(p.z) << 1);
    }
};

struct UV {
    float startX;
    float startY;
    float endX;
    float endY;
};

inline float randomFloat(float begin, float end) {
    return begin + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (end - begin)));
}

inline Vector3 operator+(Vector3 a, Vector3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3 operator-(Vector3 a, Vector3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector3& operator+=(Vector3& a, const Vector3& b) {
    a = a + b;
    return a;
}

inline Vector3& operator-=(Vector3& a, const Vector3& b) {
    a = a - b;
    return a;
}

inline Vector2 operator*(Vector2 a, float b) {
    return {a.x * b, a.y * b};
}

inline Vector2 operator+(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
}

inline Vector2& operator+=(Vector2& a, Vector2 b) {
    a = a + b;
    return a;
}

inline bool operator==(Vector3 a, Vector3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

enum class Faces {
    Up,    // y+
    Down,  // y-
    Back,  // z+
    Front, // z-
    Left,  // x+
    Right  // x-
};

inline void drawFace(const BlockPos& blockPos, Faces face, const Color& col) {
    const float awayFromBlock = .004f;

    float x0 = blockPos.x;
    float y0 = blockPos.y;
    float z0 = blockPos.z;
    float x1 = blockPos.x + 1.f;
    float y1 = blockPos.y + 1.f;
    float z1 = blockPos.z + 1.f;

    switch (face) {
    case Faces::Up:
        y1 += awayFromBlock;
        // y+
        DrawTriangle3D({x0, y1, z0}, {x0, y1, z1}, {x1, y1, z0}, col);
        DrawTriangle3D({x0, y1, z1}, {x1, y1, z1}, {x1, y1, z0}, col);
        break;

    case Faces::Down:
        y0 -= awayFromBlock;
        // y-
        DrawTriangle3D({x1, y0, z0}, {x0, y0, z1}, {x0, y0, z0}, col);
        DrawTriangle3D({x1, y0, z0}, {x1, y0, z1}, {x0, y0, z1}, col);
        break;

    case Faces::Back:
        z1 += awayFromBlock;
        // z-
        DrawTriangle3D({x0, y0, z1}, {x1, y0, z1}, {x1, y1, z1}, col);
        DrawTriangle3D({x0, y0, z1}, {x1, y1, z1}, {x0, y1, z1}, col);
        break;

    case Faces::Front:
        z0 -= awayFromBlock;
        // z+
        DrawTriangle3D({x1, y1, z0}, {x1, y0, z0}, {x0, y0, z0}, col);
        DrawTriangle3D({x0, y1, z0}, {x1, y1, z0}, {x0, y0, z0}, col);
        break;

    case Faces::Left:
        x1 += awayFromBlock;
        // x+
        DrawTriangle3D({x1, y0, z0}, {x1, y1, z0}, {x1, y0, z1}, col);
        DrawTriangle3D({x1, y1, z0}, {x1, y1, z1}, {x1, y0, z1}, col);
        break;

    case Faces::Right:
        x0 -= awayFromBlock;
        // x-
        DrawTriangle3D({x0, y0, z0}, {x0, y0, z1}, {x0, y1, z1}, col);
        DrawTriangle3D({x0, y0, z0}, {x0, y1, z1}, {x0, y1, z0}, col);
        break;
    }
}

// #define DO_TIME_MEASURING

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

inline Vector2 MeasureText2(const char* text, int fontSize) {
    Vector2 textSize = {0.0f, 0.0f};

    // Check if default font has been loaded
    if (GetFontDefault().texture.id != 0) {
        int defaultFontSize = 10; // Default Font chars height in pixel
        if (fontSize < defaultFontSize)
            fontSize = defaultFontSize;
        int spacing = fontSize / defaultFontSize;

        textSize = MeasureTextEx(GetFontDefault(), text, (float)fontSize, (float)spacing);
    }

    return textSize;
}