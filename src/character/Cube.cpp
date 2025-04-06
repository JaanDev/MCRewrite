#include <character/Cube.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Game.hpp>
#include <array>

Cube::Cube(const glm::ivec2& textureOffset) : m_pos(.0), m_rot(0), m_size(0), m_texOff(textureOffset) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttrib1f(1, 1.f);
    glDisableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glVertexAttribI1i(3, 0);
    glDisableVertexAttribArray(3);

    glBindVertexArray(0);
}

// void Cube::init(const glm::ivec2& textureOffset, const glm::vec3& pos, const glm::vec3& size, Vertex* where) { // w h d
//     // for (int x = 0; x < 2; x++) {
//     //     for (int y = 0; y < 2; y++) {
//     //         for (int z = 0; z < 2; z++) {
//     //             where->u = (textureOffset.x + ) / 64.0f;
//     //             where++;
//     //         }
//     //     }
//     // }
//     auto w = size.x;
//     auto h = size.y;
//     auto d = size.z;

//     float u0, v0, u1, v1;

//     // u1 v0
//     // u0 v0
//     // u0 v1
//     // u1 v1

//     Vertex* where_ = where;

//     // 0
//     u0 = textureOffset.x + d + w;
//     v0 = textureOffset.y + d;
//     u1 = u0 + d;
//     v1 = v0 + h;
//     where[0] = {w, 0, d, u1, v0};
//     where[1] = {w, 0, 0, u0, v0};
//     where[2] = {w, h, 0, u0, v1};
//     where[3] = {w, h, d, u1, v1};
//     where += 4;
//     // 1
//     u0 = textureOffset.x;
//     v0 = textureOffset.y + d;
//     u1 = u0 + d;
//     v1 = v0 + h;
//     where[0] = {0, 0, 0, u1, v0};
//     where[1] = {0, 0, d, u0, v0};
//     where[2] = {0, h, d, u0, v1};
//     where[3] = {0, h, 0, u1, v1};
//     where += 4;
//     // 2
//     u0 = textureOffset.x + d;
//     v0 = textureOffset.y;
//     u1 = u0 + w;
//     v1 = v0 + d;
//     where[0] = {w, 0, d, u1, v0};
//     where[1] = {0, 0, d, u0, v0};
//     where[2] = {0, 0, 0, u0, v1};
//     where[3] = {w, 0, 0, u1, v1};
//     where += 4;
//     // 4
//     u0 = textureOffset.x + d + w;
//     v0 = textureOffset.y;
//     u1 = u0 + w;
//     v1 = v0 + d;
//     where[0] = {w, h, 0, u1, v0};
//     where[1] = {0, h, 0, u0, v0};
//     where[2] = {0, h, d, u0, v1};
//     where[3] = {w, h, d, u1, v1};
//     where += 4;
//     // 5
//     u0 = textureOffset.x + d;
//     v0 = textureOffset.y + d;
//     u1 = u0 + w;
//     v1 = v0 + h;
//     where[0] = {w, 0, 0, u1, v0};
//     where[1] = {0, 0, 0, u0, v0};
//     where[2] = {0, h, 0, u0, v1};
//     where[3] = {w, h, 0, u1, v1};
//     where += 4;
//     // 6
//     u0 = textureOffset.x + d + w + d;
//     v0 = textureOffset.y + d;
//     u1 = u0 + w;
//     v1 = v0 + h;
//     where[0] = {0, 0, d, u1, v0};
//     where[1] = {w, 0, d, u0, v0};
//     where[2] = {w, h, d, u0, v1};
//     where[3] = {0, h, d, u1, v1};
//     where += 4;

//     for (int i = 0; i < 24; i++) {
//         where_[i].x += pos.x;
//         where_[i].y += pos.y;
//         where_[i].z += pos.z;
//     }
// }

void Cube::setBox(const glm::vec3& pos0, const glm::vec3& sz) {
    glm::vec3 pos1 = pos0 + sz;

    Vertex u0 = {pos0.x, pos0.y, pos0.z, 0.f, 0.f};
    Vertex u1 = {pos1.x, pos0.y, pos0.z, 0.f, 8.f};
    Vertex u2 = {pos1.x, pos1.y, pos0.z, 8.f, 8.f};
    Vertex u3 = {pos0.x, pos1.y, pos0.z, 8.f, 0.f};
    Vertex l0 = {pos0.x, pos0.y, pos1.z, 0.f, 0.f};
    Vertex l1 = {pos1.x, pos0.y, pos1.z, 0.f, 8.f};
    Vertex l2 = {pos1.x, pos1.y, pos1.z, 8.f, 8.f};
    Vertex l3 = {pos0.x, pos1.y, pos1.z, 8.f, 0.f};

    EntityPolygon vertices[] = {
        EntityPolygon({l1, u1, u2, l2}, m_texOff.x + sz.z + sz.x, m_texOff.y + sz.z, m_texOff.x + sz.z + sz.x + sz.z, m_texOff.y + sz.z + sz.y),
        EntityPolygon({u0, l0, l3, u3}, m_texOff.x + 0, m_texOff.y + sz.z, m_texOff.x + sz.z, m_texOff.y + sz.z + sz.y),
        EntityPolygon({l1, l0, u0, u1}, m_texOff.x + sz.z, m_texOff.y + 0, m_texOff.x + sz.z + sz.x, m_texOff.y + sz.z),
        EntityPolygon({u2, u3, l3, l2}, m_texOff.x + sz.z + sz.x, m_texOff.y + 0, m_texOff.x + sz.z + sz.x + sz.x, m_texOff.y + sz.z),
        EntityPolygon({u1, u0, u3, u2}, m_texOff.x + sz.z, m_texOff.y + sz.z, m_texOff.x + sz.z + sz.x, m_texOff.y + sz.z + sz.y),
        EntityPolygon({l0, l1, l2, l3}, m_texOff.x + sz.z + sz.x + sz.z, m_texOff.y + sz.z, m_texOff.x + sz.z + sz.x + sz.z + sz.x, m_texOff.y + sz.z + sz.y)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
}

void Cube::render() {
    glm::mat4 model = Game::get().getModelMatrix();
    glm::mat4 bacModel = model;
    model = glm::translate(model, m_pos);
    model = glm::rotate(model, m_rot.z, glm::vec3(0.f, 0.f, 1.f));
    model = glm::rotate(model, m_rot.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, m_rot.x, glm::vec3(1.f, 0.f, 0.f));

    // for (int x = 0; x < 2; x++) {
    //     for (int y = 0; y < 2; y++) {
    //         for (int z = 0; z < 2; z++) {
    //             glm::vec4 vertex(x * m_size.x, y * m_size.y, z * m_size.z, 1.0);
    //             vertex = model * vertex;
    //             where->x = vertex.x;
    //             where->y = vertex.y;
    //             where->z = vertex.z;
    //             where++;
    //         }
    //     }
    // }

    // aahh we need matrix stack
    Game::get().setModelMatrix(model);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);

    Game::get().setModelMatrix(bacModel);
}