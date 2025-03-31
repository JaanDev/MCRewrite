#include "Chunk.hpp"
#include <utils.hpp>

Chunk::Chunk(Level& level, const glm::ivec3& min, const glm::ivec3& max) : m_level(level), m_min(min), m_max(max), m_built(false) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 8, m_vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8, (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Chunk::render() {
    if (!m_built) {
        generateMesh();
    }

    glBindVertexArray(m_vao);
    glDrawArrays(GL_QUADS, 0, m_vertices.size());
    glBindVertexArray(0);
}

void Chunk::generateMesh() {
    m_vertices.clear();
    m_built = true;

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
}

void Chunk::renderTile(const glm::ivec3& pos, int tileID) {
    if (!m_level.isSolidTile(glm::ivec3(pos.x + 1, pos.y, pos.z))) {
        addFace(pos, Faces::Right, tileID);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x - 1, pos.y, pos.z))) {
        addFace(pos, Faces::Left, tileID);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y + 1, pos.z))) {
        addFace(pos, Faces::Up, tileID);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y - 1, pos.z))) {
        addFace(pos, Faces::Down, tileID);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y, pos.z + 1))) {
        addFace(pos, Faces::Front, tileID);
    }

    if (!m_level.isSolidTile(glm::ivec3(pos.x, pos.y, pos.z - 1))) {
        addFace(pos, Faces::Back, tileID);
    }
}

void Chunk::addFace(const glm::vec3& pos, Faces face, int tileID) {
    float minU = tileID / 16.0f;
    float maxU = minU + 16.0f / 256.0f;
    float minV = 0.0f;
    float maxV = minV + 16.0f / 256.0f;
    glm::vec3 shade = {0.6f, 1.f, 0.8f};
    float b = m_level.getBrightness(pos);

    switch (face) {
        case Faces::Front:
            b *= shade.z;
            m_vertices.insert(m_vertices.end(), {
                pos.x,     pos.y,     pos.z + 1, b, b, b, minU, minV, 
                pos.x + 1, pos.y,     pos.z + 1, b, b, b, minU, maxV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, maxU, maxV,
                pos.x,     pos.y + 1, pos.z + 1, b, b, b, maxU, minV
            });
            break;

        case Faces::Back:
            b *= shade.z;
            m_vertices.insert(m_vertices.end(), {
                pos.x,     pos.y,     pos.z, b, b, b, maxU, minV,
                pos.x,     pos.y + 1, pos.z, b, b, b, minU, minV,
                pos.x + 1, pos.y + 1, pos.z, b, b, b, minU, maxV,
                pos.x + 1, pos.y,     pos.z, b, b, b, maxU, maxV
            });
            break;

        case Faces::Left:
            b *= shade.x;
            m_vertices.insert(m_vertices.end(), {
                pos.x, pos.y,     pos.z,     b, b, b, maxU, minV,
                pos.x, pos.y,     pos.z + 1, b, b, b, minU, minV,
                pos.x, pos.y + 1, pos.z + 1, b, b, b, minU, maxV,
                pos.x, pos.y + 1, pos.z,     b, b, b, maxU, maxV
            });
            break;

        case Faces::Right:
            b *= shade.x;
            m_vertices.insert(m_vertices.end(), {
                pos.x + 1, pos.y,     pos.z,     b, b, b, minU, maxV,
                pos.x + 1, pos.y + 1, pos.z,     b, b, b, maxU, maxV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, maxU, minV,
                pos.x + 1, pos.y,     pos.z + 1, b, b, b, minU, minV
            });
            break;

        case Faces::Up:
            b *= shade.y;
            m_vertices.insert(m_vertices.end(), {
                pos.x,     pos.y + 1, pos.z,     b, b, b, maxU, maxV,
                pos.x,     pos.y + 1, pos.z + 1, b, b, b, maxU, minV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, minU, minV,
                pos.x + 1, pos.y + 1, pos.z,     b, b, b, minU, maxV
            });
            break;

        case Faces::Down:
            b *= shade.y;
            m_vertices.insert(m_vertices.end(), {
                pos.x,     pos.y, pos.z,     b, b, b, minU, maxV,
                pos.x + 1, pos.y, pos.z,     b, b, b, minU, minV,
                pos.x + 1, pos.y, pos.z + 1, b, b, b, maxU, minV,
                pos.x,     pos.y, pos.z + 1, b, b, b, maxU, maxV
            });
            break;
    }
}