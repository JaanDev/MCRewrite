#pragma once
#include <AABB.hpp>

#include <glm/fwd.hpp>
#include <vector>
#include <memory>

#include <utils.hpp>

class Chunk;

class Level {
public:
    Level(int width, int height, int depth);

    void render();

    bool isSolidTile(glm::ivec3 pos);
    void calcLightDepths(int minX, int minZ, int maxX, int maxZ);
    void rebuildChunks(glm::ivec3 min, glm::ivec3 max);

    // void setTile(glm::ivec3 pos, BlockTypes type);
    float getBrightness(const glm::ivec3& pos);
    std::vector<AABB> getCubes(const AABB& other);

    auto getWidth() const { return m_width; }
    auto getHeight() const { return m_height; }
    auto getDepth() const { return m_depth; }

private:
    std::vector<uint8_t> m_blocks;
    std::vector<int> m_lightDepths;
    std::vector<std::shared_ptr<Chunk>> m_chunks;

    int m_width;
    int m_height;
    int m_depth;
};