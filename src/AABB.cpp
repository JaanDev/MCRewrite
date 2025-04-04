#include "AABB.hpp"
#include <algorithm>

AABB::AABB(const glm::vec3& min, const glm::vec3& max) : m_min(min), m_max(max) {}

AABB AABB::expand(const glm::vec3& size) {
    auto min = m_min;
    auto max = m_max;

    if (size.x < 0.f) {
        min.x += size.x;
    }
    if (size.x > 0.f) {
        max.x += size.x;
    }

    if (size.y < 0.f) {
        min.y += size.y;
    }
    if (size.y > 0.f) {
        max.y += size.y;
    }

    if (size.z < 0.f) {
        min.z += size.z;
    }
    if (size.z > 0.f) {
        max.z += size.z;
    }

    return AABB(min, max);
}

AABB AABB::grow(const glm::vec3& size) {
    return AABB(m_min - size, m_max + size);
}

float AABB::clipXCollide(AABB& other, float xa) {
    if (other.m_max.y <= m_min.y || other.m_min.y >= m_max.y) {
        return xa;
    }

    if (other.m_max.z > m_min.z && other.m_min.z < m_max.z) {
        if (xa > 0.f && other.m_max.x <= m_min.x) {
            xa = std::min(m_min.x - other.m_max.x, xa);
        }

        if (xa < 0.f && other.m_min.x >= m_max.x) {
            xa = std::max(xa, m_max.x - other.m_min.x);
        }
    }

    return xa;
}

float AABB::clipYCollide(AABB& other, float ya) {
    if (other.m_max.x <= m_min.x || other.m_min.x >= m_max.x) {
        return ya;
    }

    if (other.m_max.z > m_min.z && other.m_min.z < m_max.z) {
        if (ya > 0.f && other.m_max.y <= m_min.y) {
            ya = std::min(ya, m_min.y - other.m_max.y);
        }

        if (ya < 0.f && other.m_min.y >= m_max.y) {
            ya = std::max(ya, m_max.y - other.m_min.y);
        }
    }

    return ya;
}

float AABB::clipZCollide(AABB& other, float za) {
    if (other.m_max.x <= m_min.x || other.m_min.x >= m_max.x) {
        return za;
    }

    if (other.m_max.y > m_min.y && other.m_min.y < m_max.y) {
        if (za > 0.f && other.m_max.z <= m_min.z) {
            za = std::min(za, m_min.z - other.m_max.z);
        }

        if (za < 0.f && other.m_min.z >= m_max.z) {
            za = std::max(za, m_max.z - other.m_min.z);
        }
    }

    return za;
}

bool AABB::intersects(AABB& other) {
    return !(other.m_max.x <= m_min.x || other.m_min.x >= m_max.x) && !(other.m_max.y <= m_min.y || other.m_min.y >= m_max.y) &&
           (other.m_max.z > m_min.z && other.m_min.z < m_max.z);
}

void AABB::move(const glm::vec3& other) {
    m_min += other;
    m_max += other;
}