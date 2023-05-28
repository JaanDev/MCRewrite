#include "Level.hpp"

Level::Level() {
    generate();
}

void Level::generate() {
    for (uint8_t x = 0; x < chunksCount; x++) {
        for (uint8_t z = 0; z < chunksCount; z++) {
            m_chunks.push_back(std::make_shared<Chunk>(ChunkPos {x, z}, this));
        }
    }

    for (const auto& chunk : m_chunks) {
        chunk->generateMesh();
    }
}

bool Level::isSolidTile(const BlockPos& pos) {
    return getBlock(pos) != BlockTypes::Air;
}

void Level::render() {
    for (const auto& chunk : m_chunks) {
        chunk->render();
    }
}

std::shared_ptr<Chunk> Level::getChunk(const ChunkPos& pos) {
    for (const auto& chunk : m_chunks) {
        if (chunk->getPos() == pos)
            return chunk;
    }

    return nullptr;
}

BlockTypes Level::getBlock(const BlockPos& pos) {
    if (pos.y >= chunkHeight)
        return BlockTypes::Air;

    auto chunk = getChunk({static_cast<int32_t>(floorf(pos.x / (float)chunkSize)), static_cast<int32_t>(floorf(pos.z / (float)chunkSize))});
    if (!chunk)
        return BlockTypes::Air;

    auto x = pos.x % chunkSize;
    if (x < 0)
        x = chunkSize - x;

    auto z = pos.z % chunkSize;
    if (z < 0)
        z = chunkSize - z;

    return chunk->getBlock({x, pos.y, z});
}

void Level::setTile(const BlockPos& pos, BlockTypes type) {
    if (pos.y >= chunkHeight)
        return;

    auto chunk = getChunk({static_cast<int32_t>(floorf(pos.x / (float)chunkSize)), static_cast<int32_t>(floorf(pos.z / (float)chunkSize))});
    if (!chunk)
        return;

    auto x = pos.x % chunkSize;
    if (x < 0)
        x = chunkSize - x;

    auto z = pos.z % chunkSize;
    if (z < 0)
        z = chunkSize - z;

    chunk->setBlock({x, pos.y, z}, type);
    chunk->generateMesh();
}
