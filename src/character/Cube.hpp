#pragma once
#include <utils.hpp>
#include <glad/glad.h>

class Cube {
public:
    Cube();

    void init(const glm::ivec2& textureOffset, const glm::vec3& pos, const glm::vec3& size, Vertex* where);
    // inline void setTexOff(const glm::ivec2& textureOffset) { m_texOff = textureOffset; }
    inline void setPos(const glm::vec3& pos) { m_pos = pos; }
    inline void setRot(const glm::vec3& rot) { m_rot = rot; }

    void render(Vertex* where);

private:
    // glm::ivec2 m_texOff;
    glm::vec3 m_pos;
    glm::vec3 m_rot;
    glm::vec3 m_size;
};