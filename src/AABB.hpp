#pragma once
#include <glm/glm.hpp>

class AABB {
public:
    AABB(const glm::vec3& min = glm::vec3(0.f), const glm::vec3& max = glm::vec3(0.f));

    AABB expand(const glm::vec3& size);
    AABB grow(const glm::vec3& size);

    float clipXCollide(AABB& other, float xa);
    float clipYCollide(AABB& other, float ya);
    float clipZCollide(AABB& other, float za);
    bool intersects(AABB& other);

    void move(const glm::vec3& delta);

    inline const glm::vec3& min() const { return m_min; }
    inline const glm::vec3& max() const { return m_max; }

private:
    glm::vec3 m_min;
    glm::vec3 m_max;
};