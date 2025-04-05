#pragma once
#include <glm/glm.hpp>

struct ChunkVertex {
    float x, y, z;
    float brightness;
    float u, v;
    int shadow;
};

struct Vertex {
    float x, y, z;
    float u, v;

    inline void remap(float u, float v) { this->u = u; this->v = v; }
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
