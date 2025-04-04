#pragma once
#include <AABB.hpp>
#include <Level.hpp>
#include <utils.hpp>
#include <glad/glad.h>
#include <vector>

#define CHUNK_SIZE 16

class Chunk {
public:
    inline static int updates = 0;
    inline static int buildThisFrame = 0;

    Chunk(Level& level, const glm::ivec3& min, const glm::ivec3& max);

    void render();
    void build();
    void renderTile(const glm::vec3& pos, int tileID);

    inline void rebuild() { m_built = false; }
    inline AABB getBounds() const { return AABB(m_min, m_max); }

private:
    Level& m_level;

    glm::ivec3 m_min;
    glm::ivec3 m_max;

    bool m_built;

    std::vector<ChunkVertex> m_vertices;
    GLuint m_vao;
    GLuint m_vbo;
};