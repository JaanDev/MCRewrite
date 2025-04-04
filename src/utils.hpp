#pragma once
#include <glm/glm.hpp>

struct ChunkVertex {
    float x, y, z;
    float brightness;
    float u, v;
    int shadow;
};

enum class Faces {
    Up,    // y+
    Down,  // y-
    Back,  // z+
    Front, // z-
    Left,  // x+
    Right  // x-
};

struct HitResult {
    bool hit = false;

    glm::ivec3 pos;
    Faces face;
};
