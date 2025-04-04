#pragma once
#include <AABB.hpp>

#include <glm/fwd.hpp>
#include <vector>
#include <memory>

#include <utils.hpp>
#include <glad/glad.h>

class Chunk;

class Level {
public:
    Level(int width, int height, int depth);

    void render(const glm::mat4& VP);
    void renderHit(const HitResult& hit);

    bool isSolidTile(const glm::ivec3& pos);
    void calcLightDepths(int minX, int minZ, int maxX, int maxZ);
    void rebuildChunks(glm::ivec3 min, glm::ivec3 max);

    void setTile(const glm::ivec3& pos, int id);
    float getBrightness(const glm::ivec3& pos);
    std::vector<AABB> getCubes(const AABB& other);

    inline auto getWidth() const { return m_width; }
    inline auto getHeight() const { return m_height; }
    inline auto getDepth() const { return m_depth; }

    void save();
    bool load();

private:
    std::vector<uint8_t> m_blocks;
    std::vector<int> m_lightDepths;
    std::vector<std::shared_ptr<Chunk>> m_chunks;

    int m_width;
    int m_height;
    int m_depth;
};