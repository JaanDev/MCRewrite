#pragma once
#include "includes.hpp"
#include "Level.hpp"

class Level;

const uint8_t chunkSize = 16; // x and z
const uint16_t chunkHeight = 256; // y

const uint8_t surfaceLevel = 10; // surface level

class Chunk {
  public:
    Chunk(const ChunkPos& pos, Level* level);
    ~Chunk();

    // in local space
    bool isSolidTile(const BlockPos& pos);
    void render();
    ChunkPos getPos();
    void generateMesh();
    BlockTypes getBlock(const BlockPos& pos);
    void setBlock(const BlockPos& pos, BlockTypes type);

  private:
    void generate();
    uint32_t getFaceCount();
    void unload();

    ChunkPos m_pos;
    Level* m_level;
    std::array<std::array<std::array<BlockTypes, chunkSize>, chunkHeight>, chunkSize> m_data;
    Mesh m_mesh;
    Model m_model;
};