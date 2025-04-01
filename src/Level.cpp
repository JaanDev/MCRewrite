#include "Level.hpp"
#include <Frustum.hpp>
#include <Chunk.hpp>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <memory>

Level::Level(int width, int height, int depth) : m_width(width), m_height(height), m_depth(depth), m_blocks(width * height * depth), m_lightDepths(width * height) {
    // Fill level with tiles
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < depth; y++) {
            for (int z = 0; z < height; z++) {
                // Calculate index from x, y and z
                int index = (y * m_height + z) * m_width + x;

                // Fill level with tiles
                m_blocks[index] = (uint8_t)((y <= depth * 2 / 3) ? 1 : 0);
            }
        }
    }

    calcLightDepths(0, 0, width, height);

    glm::ivec3 chunkCount = {m_width, m_depth, m_height};
    chunkCount /= CHUNK_SIZE;

    m_chunks.resize(chunkCount.x * chunkCount.y * chunkCount.z);

    for (int x = 0; x < chunkCount.x; x++) {
        for (int y = 0; y < chunkCount.y; y++) {
            for (int z = 0; z < chunkCount.z; z++) {
                glm::ivec3 min = {
                    x * CHUNK_SIZE,
                    y * CHUNK_SIZE,
                    z * CHUNK_SIZE
                };

                glm::ivec3 max = {
                    std::min(m_width, (x + 1) * CHUNK_SIZE),
                    std::min(m_depth, (y + 1) * CHUNK_SIZE),
                    std::min(m_height, (z + 1) * CHUNK_SIZE)
                };

                m_chunks[(x + y * chunkCount.x) * chunkCount.z + z] = std::make_shared<Chunk>(*this, min, max);
            }
        }
    }
}

bool Level::isSolidTile(glm::ivec3 pos) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= m_width || pos.y >= m_depth || pos.z >= m_height) {
        return false;
    }

    int index = (pos.y * m_height + pos.z) * m_width + pos.x;

    return m_blocks[index] != 0;
}

void Level::render() {
    auto frustum = Frustum::get();

    for (const auto& chunk : m_chunks) {
        // if (frustum->cubeInFrustum(chunk->getBounds())) {
            chunk->render();
        // }
    }
}

void Level::calcLightDepths(int minX, int minZ, int maxX, int maxZ) {
    for (int x = minX; x < minX + maxX; x++) {
        for (int z = minZ; z < minZ + maxZ; z++) {
            int prevDepth = m_lightDepths[x + z * m_width];

            int depth = m_depth - 1;
            while (depth > 0 && !isSolidTile(glm::vec3(x, m_depth, z))) {
                depth--;
            }

            m_lightDepths[x + z * m_width] = depth;

            if (prevDepth != depth) {
                int minTileChangeY = std::min(prevDepth, depth);
                int maxTileChangeY = std::max(prevDepth, depth);


                rebuildChunks(glm::ivec3(x - 1, minTileChangeY - 1, z - 1), glm::ivec3(x + 1, maxTileChangeY + 1, z + 1));
            }
        }
    }
}

void Level::rebuildChunks(glm::ivec3 min, glm::ivec3 max) {
    min /= CHUNK_SIZE;
    max /= CHUNK_SIZE;

    min.x = std::max(min.x, 0);
    min.y = std::max(min.y, 0);
    min.z = std::max(min.z, 0);

    max.x = std::min(max.x, m_width / CHUNK_SIZE - 1);
    max.y = std::min(max.y, m_depth / CHUNK_SIZE - 1);
    max.z = std::min(max.z, m_height / CHUNK_SIZE - 1);

    for (int x = min.x; x <= max.x; x++) {
        for (int y = min.y; y <= max.y; y++) {
            for (int z = min.z; z <= max.z; z++) {
                auto chunk = m_chunks[(x + y * (m_width / CHUNK_SIZE)) * (m_height / CHUNK_SIZE) + z];
                chunk->rebuild();
            }
        }
    }
}

// void Level::setTile(const BlockPos& pos, BlockTypes type) {
//     if (pos.y >= chunkHeight)
//         return;

//     auto chunk = getChunk(pos);
//     if (!chunk)
//         return;

//     auto chunkPos = chunk->getPos();

//     chunk->setBlock(pos.local(), type);
//     chunk->calcLightDepths();
//     chunk->generateMesh();

//     auto localPos = pos.local();

//     if (localPos.x == 0) {
//         if (auto c = getChunk(ChunkPos {chunkPos.x - 1, chunkPos.z}))
//             c->generateMesh();
//     } else if (localPos.x == chunkSize - 1)
//         if (auto c = getChunk(ChunkPos {chunkPos.x + 1, chunkPos.z}))
//             c->generateMesh();

//     if (localPos.z == 0) {
//         if (auto c = getChunk(ChunkPos {chunkPos.x, chunkPos.z - 1}))
//             c->generateMesh();
//     } else if (localPos.z == chunkSize - 1)
//         if (auto c = getChunk(ChunkPos {chunkPos.x, chunkPos.z + 1}))
//             c->generateMesh();
// }

float Level::getBrightness(const glm::ivec3& pos) {
    float dark = 0.8f;
    float light = 1.0f;

    if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= m_width || pos.y >= m_depth || pos.z >= m_height) {
        return light;
    }

    if (pos.y < m_lightDepths[pos.x + pos.z * m_width]) {
        return dark;
    }

    return light;
}

std::vector<AABB> Level::getCubes(const AABB& other) {
    std::vector<AABB> aabbs;

    auto pos0 = other.min();
    auto pos1 = other.max() + 1.f;

    if (pos0.x < 0) {
        pos0.x = 0;
    }

    if (pos0.y < 0) {
        pos0.y = 0;
    }

    if (pos0.z < 0) {
        pos0.z = 0;
    }

    if (pos1.x > m_width) {
        pos1.x = m_width;
    }

    if (pos1.y > m_depth) {
        pos1.y = m_depth;
    }
    
    if (pos1.z > m_height) {
        pos1.z = m_height;
    }

    for (int x = pos0.x; x < pos1.x; x++) {
        for (int y = pos0.y; y < pos1.y; y++) {
            for (int z = pos0.z; z < pos1.z; z++) {
                if (isSolidTile({x, y, z})) {
                    aabbs.push_back(AABB(glm::vec3(x, y, z), glm::vec3(x, y, z) + 1.f));
                }
            }
        }
    }

    return aabbs;
}
