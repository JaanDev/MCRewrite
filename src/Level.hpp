#pragma once
#include "includes.hpp"
#include "Chunk.hpp"

class Chunk;

const uint8_t chunksCount = 4;

class Level {
  public:
    Level();

    void generate();
    bool isSolidTile(const BlockPos& pos);
    void render();
    std::shared_ptr<Chunk> getChunk(const ChunkPos& pos);
    BlockTypes getBlock(const BlockPos& pos);
    void setTile(const BlockPos& pos, BlockTypes type);

  private:
    std::vector<std::shared_ptr<Chunk>> m_chunks;
};