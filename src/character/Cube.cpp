#include <character/Cube.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Game.hpp>

Cube::Cube() : m_pos(0), m_rot(0), m_size(0) {}

void Cube::init(const glm::ivec2& textureOffset, const glm::vec3& pos, const glm::vec3& size, Vertex* where) { // w h d
    // for (int x = 0; x < 2; x++) {
    //     for (int y = 0; y < 2; y++) {
    //         for (int z = 0; z < 2; z++) {
    //             where->u = (textureOffset.x + ) / 64.0f;
    //             where++;
    //         }
    //     }
    // }
    auto w = size.x;
    auto h = size.y;
    auto d = size.z;

    float u0, v0, u1, v1;

    // u1 v0
    // u0 v0
    // u0 v1
    // u1 v1

    Vertex* where_ = where;

    // 0
    u0 = textureOffset.x + d + w;
    v0 = textureOffset.y + d;
    u1 = u0 + d;
    v1 = v0 + h;
    where[0] = {w, 0, d, u1, v0};
    where[1] = {w, 0, 0, u0, v0};
    where[2] = {w, h, 0, u0, v1};
    where[3] = {w, h, d, u1, v1};
    where += 4;
    // 1
    u0 = textureOffset.x;
    v0 = textureOffset.y + d;
    u1 = u0 + d;
    v1 = v0 + h;
    where[0] = {0, 0, 0, u1, v0};
    where[1] = {0, 0, d, u0, v0};
    where[2] = {0, h, d, u0, v1};
    where[3] = {0, h, 0, u1, v1};
    where += 4;
    // 2
    u0 = textureOffset.x + d;
    v0 = textureOffset.y;
    u1 = u0 + w;
    v1 = v0 + d;
    where[0] = {w, 0, d, u1, v0};
    where[1] = {0, 0, d, u0, v0};
    where[2] = {0, 0, 0, u0, v1};
    where[3] = {w, 0, 0, u1, v1};
    where += 4;
    // 4
    u0 = textureOffset.x + d + w;
    v0 = textureOffset.y;
    u1 = u0 + w;
    v1 = v0 + d;
    where[0] = {w, h, 0, u1, v0};
    where[1] = {0, h, 0, u0, v0};
    where[2] = {0, h, d, u0, v1};
    where[3] = {w, h, d, u1, v1};
    where += 4;
    // 5
    u0 = textureOffset.x + d;
    v0 = textureOffset.y + d;
    u1 = u0 + w;
    v1 = v0 + h;
    where[0] = {w, 0, 0, u1, v0};
    where[1] = {0, 0, 0, u0, v0};
    where[2] = {0, h, 0, u0, v1};
    where[3] = {w, h, 0, u1, v1};
    where += 4;
    // 6
    u0 = textureOffset.x + d + w + d;
    v0 = textureOffset.y + d;
    u1 = u0 + w;
    v1 = v0 + h;
    where[0] = {0, 0, d, u1, v0};
    where[1] = {w, 0, d, u0, v0};
    where[2] = {w, h, d, u0, v1};
    where[3] = {0, h, d, u1, v1};
    where += 4;

    for (int i = 0; i < 24; i++) {
        where_[i].x += pos.x;
        where_[i].y += pos.y;
        where_[i].z += pos.z;
    }
}

void Cube::render(Vertex* where) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_pos);
    model = glm::rotate(model, glm::radians(m_rot.z), glm::vec3(0.f, 0.f, 1.f));
    model = glm::rotate(model, glm::radians(m_rot.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(m_rot.x), glm::vec3(1.f, 0.f, 0.f));

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int z = 0; z < 2; z++) {
                glm::vec4 vertex(x * m_size.x, y * m_size.y, z * m_size.z, 1.0);
                vertex = model * vertex;
                where->x = vertex.x;
                where->y = vertex.y;
                where->z = vertex.z;
                where++;
            }
        }
    }

    // // aahh we need matrix stack
    // Game::get().setModelMatrix(model);
}