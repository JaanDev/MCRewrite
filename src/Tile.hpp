#pragma once
#include <utils.hpp>
#include <vector>
#include <Level.hpp>
#include <glm/glm.hpp>


namespace Tile {
    void renderFace(std::vector<float>& vertices, Level& level, int tileID, const glm::vec3& pos, Faces face);
    void renderFaceMin(std::vector<float>& vertices, const glm::vec3& pos, Faces face);
}