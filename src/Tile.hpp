#pragma once
#include <utils.hpp>
#include <vector>
#include <Level.hpp>
#include <glm/glm.hpp>


namespace Tile {
    void renderFace(std::vector<ChunkVertex>& vertices, Level& level, int tileID, const glm::vec3& pos, Faces face);
}