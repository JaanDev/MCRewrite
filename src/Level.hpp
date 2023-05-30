#pragma once
#include "includes.hpp"
#include "Chunk.hpp"
#include "AABB.hpp"

class Chunk;

class Level {
  public:
    Level();

    void generate();
    bool isSolidTile(const BlockPos& pos);
    void render();
    std::shared_ptr<Chunk> getChunk(const ChunkPos& pos);
    std::shared_ptr<Chunk> getChunk(const BlockPos& pos);
    BlockTypes getBlock(const BlockPos& pos);
    void setTile(const BlockPos& pos, BlockTypes type);
    float getBrightness(const BlockPos& pos);
    vector<std::shared_ptr<AABB>> getCubes(std::shared_ptr<AABB> other);

  private:
    std::vector<std::shared_ptr<Chunk>> m_chunks;
};