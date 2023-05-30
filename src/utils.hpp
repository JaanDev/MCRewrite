#pragma once
#include "includes.hpp"

const uint8_t chunkSize = 16;     // x and z
const uint16_t chunkHeight = 256; // y
const uint8_t chunksCount = 4;

const uint8_t surfaceLevel = 10; // surface level

const float mouseSensitivity = .003f;

enum class BlockTypes : uint8_t {
    Air = 0,
    Rock,
    Grass,
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
};

struct ChunkPos {
    int32_t x;
    int32_t z;

    bool operator==(const ChunkPos& other) {
        return x == other.x && z == other.z;
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