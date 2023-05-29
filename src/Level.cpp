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
        chunk->calcLightDepths();
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

std::shared_ptr<Chunk> Level::getChunk(const BlockPos& pos) {
    if (pos.y >= chunkHeight)
        return nullptr;

    return getChunk(
        ChunkPos {static_cast<int32_t>(floorf(pos.x / (float)chunkSize)), static_cast<int32_t>(floorf(pos.z / (float)chunkSize))});
}

BlockTypes Level::getBlock(const BlockPos& pos) {
    if (pos.y >= chunkHeight)
        return BlockTypes::Air;

    auto chunk = getChunk(pos);
    if (!chunk)
        return BlockTypes::Air;

    return chunk->getBlock(pos.local());
}

void Level::setTile(const BlockPos& pos, BlockTypes type) {
    if (pos.y >= chunkHeight)
        return;

    auto chunk = getChunk(pos);
    if (!chunk)
        return;

    chunk->setBlock(pos.local(), type);
    chunk->calcLightDepths();
    chunk->generateMesh();
}

float Level::getBrightness(const BlockPos& pos) {
    if (pos.y >= chunkHeight)
        return 1.f;

    auto chunk = getChunk(pos);
    if (!chunk)
        return 1.f;

    return chunk->getBrightness(pos.local());
}
