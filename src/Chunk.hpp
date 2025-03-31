#pragma once
#include <AABB.hpp>
#include <Level.hpp>
#include <utils.hpp>
#include <glad/glad.h>
#include <vector>

class Chunk {
public:
    Chunk(Level& level, const glm::ivec3& min, const glm::ivec3& max);

    void render();
    void generateMesh();
    void renderTile(const glm::ivec3& pos, int tileID);
    void addFace(const glm::vec3& pos, Faces face, int tileID);

    inline void rebuild() { m_built = false; }
    inline const AABB getBounds() { return AABB(m_min, m_max); }

private:
    Level& m_level;
    glm::ivec3 m_min;
    glm::ivec3 m_max;
    bool m_built;
    std::vector<float> m_vertices;
    GLuint m_vao;
    GLuint m_vbo;
};