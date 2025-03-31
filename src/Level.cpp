#include "Level.hpp"
#include <Frustum.hpp>
#include <Chunk.hpp>

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
        if (frustum->cubeInFrustum(chunk->getBounds())) {
            chunk->render();
        }
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

                // // Notify tile column changed
                // for (auto& levelListener : m_levelListeners) {
                //     levelListener->lightColumnChanged(x, z, minTileChangeY, maxTileChangeY);
                // }
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

    // auto pos0 = other->getPos0();
    // auto pos1 = other->getPos1() + Vector3 {1.f, 1.f, 1.f};

    // if (pos0.x < 0)
    //     pos0.x = 0;
    // if (pos0.y < 0)
    //     pos0.y = 0;
    // if (pos0.z < 0)
    //     pos0.z = 0;

    // if (pos1.x > chunksCount * chunkSize)
    //     pos1.x = chunksCount * chunkSize;
    // if (pos1.y > chunkHeight)
    //     pos1.y = chunkHeight;
    // if (pos1.z > chunksCount * chunkSize)
    //     pos1.z = chunksCount * chunkSize;

    // for (int x = pos0.x; x < pos1.x; x++) {
    //     for (int y = pos0.y; y < pos1.y; y++) {
    //         for (int z = pos0.z; z < pos1.z; z++) {
    //             if (isSolidTile({x, y, z}))
    //                 aabbs.push_back(
    //                     std::make_shared<AABB>(Vector3 {(float)x, (float)y, (float)z}, Vector3 {(float)x + 1, (float)y + 1, (float)z + 1}));
    //         }
    //     }
    // }

    return aabbs;
}
