#include "Level.hpp"

Level::Level() {
    generate();
}

void Level::generate() {
    for (uint8_t x = 0; x < chunksCount; x++) {
        for (uint8_t z = 0; z < chunksCount; z++) {
            m_chunks.insert({{x, z}, std::make_shared<Chunk>(ChunkPos {x, z}, this)});
        }
    }

    for (const auto& [pos, chunk] : m_chunks) {
        chunk->calcLightDepths();
        chunk->generateMesh();
    }
}

bool Level::isSolidTile(const BlockPos& pos) {
    return getBlock(pos) != BlockTypes::Air;
}

void Level::render() {
    for (const auto& [pos, chunk] : m_chunks) {
        chunk->render();
    }
}

std::shared_ptr<Chunk> Level::getChunk(const ChunkPos& pos) {
    return m_chunks.count(pos) != 0 ? m_chunks[pos] : nullptr;
}

std::shared_ptr<Chunk> Level::getChunk(const BlockPos& pos) {
    return getChunk(pos.chunkPos());
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

    auto chunkPos = chunk->getPos();

    chunk->setBlock(pos.local(), type);
    chunk->calcLightDepths();
    chunk->generateMesh();

    auto localPos = pos.local();

    if (localPos.x == 0) {
        if (auto c = getChunk(ChunkPos {chunkPos.x - 1, chunkPos.z}))
            c->generateMesh();
    } else if (localPos.x == chunkSize - 1)
        if (auto c = getChunk(ChunkPos {chunkPos.x + 1, chunkPos.z}))
            c->generateMesh();

    if (localPos.z == 0) {
        if (auto c = getChunk(ChunkPos {chunkPos.x, chunkPos.z - 1}))
            c->generateMesh();
    } else if (localPos.z == chunkSize - 1)
        if (auto c = getChunk(ChunkPos {chunkPos.x, chunkPos.z + 1}))
            c->generateMesh();
}

float Level::getBrightness(const BlockPos& pos) {
    if (pos.y >= chunkHeight)
        return 1.f;

    auto chunk = getChunk(pos);
    if (!chunk)
        return 1.f;

    return chunk->getBrightness(pos.local());
}

vector<std::shared_ptr<AABB>> Level::getCubes(std::shared_ptr<AABB> other) {
    vector<std::shared_ptr<AABB>> aabbs;

    auto pos0 = other->getPos0();
    auto pos1 = other->getPos1() + Vector3 {1.f, 1.f, 1.f};

    if (pos0.x < 0)
        pos0.x = 0;
    if (pos0.y < 0)
        pos0.y = 0;
    if (pos0.z < 0)
        pos0.z = 0;

    if (pos1.x > chunksCount * chunkSize)
        pos1.x = chunksCount * chunkSize;
    if (pos1.y > chunkHeight)
        pos1.y = chunkHeight;
    if (pos1.z > chunksCount * chunkSize)
        pos1.z = chunksCount * chunkSize;

    for (int x = pos0.x; x < pos1.x; x++) {
        for (int y = pos0.y; y < pos1.y; y++) {
            for (int z = pos0.z; z < pos1.z; z++) {
                if (isSolidTile({x, y, z}))
                    aabbs.push_back(
                        std::make_shared<AABB>(Vector3 {(float)x, (float)y, (float)z}, Vector3 {(float)x + 1, (float)y + 1, (float)z + 1}));
            }
        }
    }

    return aabbs;
}
