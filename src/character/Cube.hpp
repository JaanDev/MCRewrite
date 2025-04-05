#pragma once
#include <array>
#include <utils.hpp>
#include <character/Polygon.hpp>
#include <vector>
#include <glad/glad.h>

class Cube {
public:
    Cube(const glm::ivec2& textureOffset);
    
    void setTexOff(const glm::ivec2& textureOffset) { m_texOff = textureOffset; }
    void setPos(const glm::vec3& pos) { m_pos = pos; }

    void addBox(const glm::vec3& pos, const glm::ivec3& size);
    void render();
private:
    GLuint m_vao;
    GLuint m_vbo;
    glm::ivec2 m_texOff;
    glm::vec3 m_pos;
    glm::vec3 m_rot;
};