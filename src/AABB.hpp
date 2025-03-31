#pragma once
#include <glm.hpp>

class AABB {
public:
    AABB(glm::vec3 min, glm::vec3 max);

    AABB expand(glm::vec3 size);
    AABB grow(glm::vec3 size);

    float clipXCollide(AABB& other, float xa);
    float clipYCollide(AABB& other, float ya);
    float clipZCollide(AABB& other, float za);
    bool intersects(AABB& other);
    
    void move(glm::vec3 delta);

    glm::vec3 min() const { return m_min; }
    glm::vec3 max() const { return m_max; }

private:
    glm::vec3 m_min;
    glm::vec3 m_max;
};