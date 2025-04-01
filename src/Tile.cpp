#include <Tile.hpp>

void Tile::renderFace(std::vector<float>& vertices, Level& level, int tileID, const glm::vec3& pos, Faces face) {
    float minU = tileID / 16.0f;
    float maxU = minU + 16.0f / 256.0f;
    float minV = 0.0f;
    float maxV = minV + 16.0f / 256.0f;
    glm::vec3 shade = {0.9f, 1.f, 0.9f};
    float b;

    switch (face) {
        case Faces::Front:
            b = level.getBrightness(glm::vec3(pos.x, pos.y, pos.z + 1)) * shade.z;
            vertices.insert(vertices.end(), {
                pos.x,     pos.y,     pos.z + 1, b, b, b, minU, minV, 
                pos.x + 1, pos.y,     pos.z + 1, b, b, b, minU, maxV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, maxU, maxV,
                pos.x,     pos.y + 1, pos.z + 1, b, b, b, maxU, minV
            });
            break;

        case Faces::Back:
            b = level.getBrightness(glm::vec3(pos.x, pos.y, pos.z - 1)) * shade.z;
            vertices.insert(vertices.end(), {
                pos.x,     pos.y,     pos.z, b, b, b, maxU, minV,
                pos.x,     pos.y + 1, pos.z, b, b, b, minU, minV,
                pos.x + 1, pos.y + 1, pos.z, b, b, b, minU, maxV,
                pos.x + 1, pos.y,     pos.z, b, b, b, maxU, maxV
            });
            break;

        case Faces::Left:
            b = level.getBrightness(glm::vec3(pos.x - 1, pos.y, pos.z)) * shade.x;
            vertices.insert(vertices.end(), {
                pos.x, pos.y,     pos.z,     b, b, b, maxU, minV,
                pos.x, pos.y,     pos.z + 1, b, b, b, minU, minV,
                pos.x, pos.y + 1, pos.z + 1, b, b, b, minU, maxV,
                pos.x, pos.y + 1, pos.z,     b, b, b, maxU, maxV
            });
            break;

        case Faces::Right:
            b = level.getBrightness(glm::vec3(pos.x + 1, pos.y, pos.z)) * shade.x;
            vertices.insert(vertices.end(), {
                pos.x + 1, pos.y,     pos.z,     b, b, b, minU, maxV,
                pos.x + 1, pos.y + 1, pos.z,     b, b, b, maxU, maxV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, maxU, minV,
                pos.x + 1, pos.y,     pos.z + 1, b, b, b, minU, minV
            });
            break;

        case Faces::Up:
            b = level.getBrightness(glm::vec3(pos.x, pos.y + 1, pos.z)) * shade.y;
            vertices.insert(vertices.end(), {
                pos.x,     pos.y + 1, pos.z,     b, b, b, maxU, maxV,
                pos.x + 1, pos.y + 1, pos.z,     b, b, b, maxU, minV,
                pos.x + 1, pos.y + 1, pos.z + 1, b, b, b, minU, minV,
                pos.x,     pos.y + 1, pos.z + 1, b, b, b, minU, maxV
            });
            break;

        case Faces::Down:
            b = level.getBrightness(glm::vec3(pos.x, pos.y - 1, pos.z)) * shade.y;
            vertices.insert(vertices.end(), {
                pos.x,     pos.y, pos.z,     b, b, b, minU, maxV,
                pos.x + 1, pos.y, pos.z,     b, b, b, minU, minV,
                pos.x + 1, pos.y, pos.z + 1, b, b, b, maxU, minV,
                pos.x,     pos.y, pos.z + 1, b, b, b, maxU, maxV
            });
            break;
    }
}

void Tile::renderFaceMin(std::vector<float>& vertices, const glm::vec3& pos, Faces face) {
    switch (face) {
        case Faces::Front:
            vertices.insert(vertices.end(), {
                pos.x,     pos.y,     pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y,     pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x,     pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f
            });
            break;

        case Faces::Back:
            vertices.insert(vertices.end(), {
                pos.x,     pos.y,     pos.z, 1.0f, 1.0f, 1.0f,
                pos.x,     pos.y + 1, pos.z, 1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z, 1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y,     pos.z, 1.0f, 1.0f, 1.0f
            });
            break;

        case Faces::Left:
            vertices.insert(vertices.end(), {
                pos.x, pos.y,     pos.z,     1.0f, 1.0f, 1.0f,
                pos.x, pos.y,     pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x, pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x, pos.y + 1, pos.z    , 1.0f, 1.0f, 1.0f
            });
            break;

        case Faces::Right:
            vertices.insert(vertices.end(), {
                pos.x + 1, pos.y,     pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y,     pos.z + 1, 1.0f, 1.0f, 1.0f
            });
            break;

        case Faces::Up:
            vertices.insert(vertices.end(), {
                pos.x,     pos.y + 1, pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x,     pos.y + 1, pos.z + 1, 1.0f, 1.0f, 1.0f
            });
            break;

        case Faces::Down:
            vertices.insert(vertices.end(), {
                pos.x,     pos.y, pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y, pos.z,     1.0f, 1.0f, 1.0f,
                pos.x + 1, pos.y, pos.z + 1, 1.0f, 1.0f, 1.0f,
                pos.x,     pos.y, pos.z + 1, 1.0f, 1.0f, 1.0f
            });
            break;
    }
}