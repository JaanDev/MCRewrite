#pragma once
#include "includes.hpp"

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