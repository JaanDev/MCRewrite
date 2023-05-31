#pragma once
#include "includes.hpp"
#include "Level.hpp"

class Level;

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
    float getBrightness(const BlockPos& pos);
    void calcLightDepths();
    Mesh* getMesh();
    Model* getModel();

  private:
    void generate();
    uint32_t getFaceCount();
    void unload();

    ChunkPos m_pos;
    Level* m_level;
    std::array<std::array<std::array<BlockTypes, chunkSize>, chunkHeight>, chunkSize> m_data;
    std::array<std::array<float, chunkSize>, chunkSize> m_lightDephts;
    Mesh m_mesh;
    Model m_model;
    bool m_built;
};