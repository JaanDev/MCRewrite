#include <Tile.hpp>

void Tile::renderFace(std::vector<ChunkVertex>& vertices, Level& level, int tileID, const glm::vec3& pos, Faces face) {
    float minU = tileID / 16.0f;
    float maxU = minU + 16.0f / 256.0f;
    float minV = 0.0f;
    float maxV = minV + 16.0f / 256.0f;
    glm::vec3 shade = {0.6f, 1.f, 0.8f};
    float brightness;
    bool shadow;

    switch (face) {
        case Faces::Front:
            brightness = level.getBrightness(glm::vec3(pos.x, pos.y, pos.z + 1)) * shade.z;
            shadow = !(brightness == shade.z);

            vertices.insert(vertices.end(), {
                {pos.x,     pos.y,     pos.z + 1, brightness, brightness, brightness, minU, maxV, shadow},
                {pos.x + 1, pos.y,     pos.z + 1, brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x + 1, pos.y + 1, pos.z + 1, brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x,     pos.y + 1, pos.z + 1, brightness, brightness, brightness, minU, minV, shadow}
            });
            break;

        case Faces::Back:
            brightness = level.getBrightness(glm::vec3(pos.x, pos.y, pos.z - 1)) * shade.z;
            shadow = !(brightness == shade.z);

            vertices.insert(vertices.end(), {
                {pos.x,     pos.y,     pos.z, brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x,     pos.y + 1, pos.z, brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x + 1, pos.y + 1, pos.z, brightness, brightness, brightness, minU, minV, shadow},
                {pos.x + 1, pos.y,     pos.z, brightness, brightness, brightness, minU, maxV, shadow}
            });
            break;

        case Faces::Left:
            brightness = level.getBrightness(glm::vec3(pos.x - 1, pos.y, pos.z)) * shade.x;
            shadow = !(brightness == shade.x);

            vertices.insert(vertices.end(), {
                {pos.x, pos.y,     pos.z,     brightness, brightness, brightness, minU, maxV, shadow},
                {pos.x, pos.y,     pos.z + 1, brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x, pos.y + 1, pos.z + 1, brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x, pos.y + 1, pos.z,     brightness, brightness, brightness, minU, minV, shadow}
            });
            break;

        case Faces::Right:
            brightness = level.getBrightness(glm::vec3(pos.x + 1, pos.y, pos.z)) * shade.x;
            shadow = !(brightness == shade.x);

            vertices.insert(vertices.end(), {
                {pos.x + 1, pos.y,     pos.z,     brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x + 1, pos.y + 1, pos.z,     brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x + 1, pos.y + 1, pos.z + 1, brightness, brightness, brightness, minU, minV, shadow},
                {pos.x + 1, pos.y,     pos.z + 1, brightness, brightness, brightness, minU, maxV, shadow}
            });
            break;

        case Faces::Up:
            brightness = level.getBrightness(glm::vec3(pos.x, pos.y + 1, pos.z)) * shade.y;
            shadow = !(brightness == shade.y);

            vertices.insert(vertices.end(), {
                {pos.x,     pos.y + 1, pos.z + 1, brightness, brightness, brightness, minU, maxV, shadow},
                {pos.x + 1, pos.y + 1, pos.z + 1, brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x + 1, pos.y + 1, pos.z,     brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x,     pos.y + 1, pos.z,     brightness, brightness, brightness, minU, minV, shadow}
            });
            break;

        case Faces::Down:
            brightness = level.getBrightness(glm::vec3(pos.x, pos.y - 1, pos.z)) * shade.y;
            shadow = !(brightness == shade.y);

            vertices.insert(vertices.end(), {
                {pos.x,     pos.y, pos.z,     brightness, brightness, brightness, minU, minV, shadow},
                {pos.x + 1, pos.y, pos.z,     brightness, brightness, brightness, maxU, minV, shadow},
                {pos.x + 1, pos.y, pos.z + 1, brightness, brightness, brightness, maxU, maxV, shadow},
                {pos.x,     pos.y, pos.z + 1, brightness, brightness, brightness, minU, maxV, shadow}
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