#pragma once
#include <glm/glm.hpp>
#include <array>

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

struct EntityPolygon {
    std::array<Vertex, 4> verts;

    EntityPolygon(const std::array<Vertex, 4>& verts, int u0, int v0, int u1, int v1) {
        this->verts = verts;
        this->verts[0].remap(u1 / 64.f, v0 / 32.f);
        this->verts[1].remap(u0 / 64.f, v0 / 32.f);
        this->verts[2].remap(u0 / 64.f, v1 / 32.f);
        this->verts[3].remap(u1 / 64.f, v1 / 32.f);

        std::reverse(this->verts.begin(), this->verts.end());
    }
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
