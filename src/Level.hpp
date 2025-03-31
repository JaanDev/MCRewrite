#pragma once
#include <Chunk.hpp>
#include <AABB.hpp>

#include <vector>
#include <memory>

class Chunk;

class Level {
public:
    Level();

    // void render();
    // void generate();

    // bool isSolidTile(glm::ivec3 pos);

    // std::shared_ptr<Chunk> getChunk(glm::ivec3 pos);
    // BlockTypes getBlock(glm::ivec3 pos);

    // void setTile(glm::ivec3 pos, BlockTypes type);
    // float getBrightness(glm::ivec3 pos);
    std::vector<AABB> getCubes(const AABB& other);

private:
    std::vector<std::shared_ptr<Chunk>> m_chunks;
};