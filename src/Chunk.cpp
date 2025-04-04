#include "Chunk.hpp"
#include <utils.hpp>
#include <Tile.hpp>

Chunk::Chunk(Level& level, const glm::ivec3& min, const glm::ivec3& max) : m_level(level), m_min(min), m_max(max), m_built(false) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, brightness));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, u));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(ChunkVertex), (void*)offsetof(ChunkVertex, shadow));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Chunk::render() {
    if (!m_built) {
        build();
    }

    glBindVertexArray(m_vao);
    glDrawArrays(GL_QUADS, 0, m_vertices.size());
    glBindVertexArray(0);
}

void Chunk::build() {
    if (buildThisFrame == 2) {
        return;
    }

    m_built = true;
    updates++;
    buildThisFrame++;
    m_vertices.clear();

    for (int x = m_min.x; x < m_max.x; x++) {
        for (int y = m_min.y; y < m_max.y; y++) {
            for (int z = m_min.z; z < m_max.z; z++) {
                // Is a tile at this location?
                if (m_level.isSolidTile({x, y, z})) {
                    int id = (y != m_level.getDepth() * 2 / 3) ? 1 : 0;

                    renderTile(glm::ivec3(x, y, z), id);
                }
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(ChunkVertex), m_vertices.data(), GL_DYNAMIC_DRAW);
}

void Chunk::renderTile(const glm::vec3& pos, int tileID) {
    if (!m_level.isSolidTile(glm::ivec3(pos.x + 1, pos.y, pos.z))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Right);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x - 1, pos.y, pos.z))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Left);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y + 1, pos.z))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Up);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y - 1, pos.z))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Down);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y, pos.z + 1))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Front);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y, pos.z - 1))) {
        Tile::renderFace(m_vertices, m_level, tileID, pos, Faces::Back);
    }
}