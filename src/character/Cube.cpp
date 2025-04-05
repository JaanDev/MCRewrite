#include <character/Cube.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Game.hpp>

Cube::Cube(const glm::ivec2& textureOffset) : m_texOff(textureOffset) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Cube::addBox(const glm::vec3& pos0, const glm::ivec3& size) {
    glm::vec3 pos1 = pos0 + glm::vec3(size);

    Vertex u0 = {pos0.x, pos0.y, pos0.z, 0.0f, 0.0f};
    Vertex u1 = {pos1.x, pos0.y, pos0.z, 0.0f, 8.0f};
    Vertex u2 = {pos1.x, pos1.y, pos0.z, 8.0f, 8.0f};
    Vertex u3 = {pos0.x, pos1.y, pos0.z, 8.0f, 0.0f};
    Vertex l0 = {pos0.x, pos0.y, pos1.z, 0.0f, 0.0f};
    Vertex l1 = {pos1.x, pos0.y, pos1.z, 0.0f, 8.0f};
    Vertex l2 = {pos1.x, pos1.y, pos1.z, 8.0f, 8.0f};
    Vertex l3 = {pos0.x, pos1.y, pos1.z, 8.0f, 0.0f};

    std::array<Polygon, 6> polygons = {
        Polygon({l1, u1, u2, l2}, m_texOff.x + size.z + size.x, m_texOff.y + size.z, m_texOff.x + size.z + size.z + size.z, m_texOff.y + size.z + size.y),
        Polygon({u0, l0, l3, u3}, m_texOff.x + 0, m_texOff.y + size.z, m_texOff.x + size.z, m_texOff.y + size.z + size.y),
        Polygon({l1, l0, u0, u1}, m_texOff.x + size.z, m_texOff.y + 0, m_texOff.x + size.z + size.x, m_texOff.y + size.z),
        Polygon({u2, u3, l3, l2}, m_texOff.x + size.z + size.x, m_texOff.y + 0, m_texOff.x + size.z + size.x + size.x, m_texOff.y + size.z),
        Polygon({u1, u0, u3, u2}, m_texOff.x + size.z, m_texOff.y + size.z, m_texOff.x + size.z + size.x, m_texOff.y + size.z + size.y),
        Polygon({l0, l1, l2, l3}, m_texOff.x + size.z + size.z + size.z, m_texOff.y + size.z, m_texOff.x + size.z + size.x + size.z + size.x, m_texOff.y + size.z + size.y)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, polygons.size() * sizeof(Polygon), polygons.data(), GL_STATIC_DRAW);
}

void Cube::render() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_pos);
    model = glm::rotate(model, m_rot.z * 57.29578f, glm::vec3(0.f, 0.f, 1.f));
    model = glm::rotate(model, m_rot.y * 57.29578f, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, m_rot.x * 57.29578f, glm::vec3(1.f, 0.f, 0.f));

    // aahh we need matrix stack
    Game::get().setModelMatrix(model);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);
}