#include "AABB.hpp"

AABB::AABB(const Vector3& pos0, const Vector3& pos1) : m_pos0(pos0), m_pos1(pos1) {}

std::shared_ptr<AABB> AABB::expand(const Vector3& size) {
    auto pos0 = m_pos0;
    auto pos1 = m_pos1;

    if (size.x < 0.f)
        pos0.x += size.x;
    if (size.x > 0.f)
        pos1.x += size.x;

    if (size.y < 0.f)
        pos0.y += size.y;
    if (size.y > 0.f)
        pos1.y += size.y;

    if (size.z < 0.f)
        pos0.z += size.z;
    if (size.z > 0.f)
        pos1.z += size.z;

    return std::make_shared<AABB>(pos0, pos1);
}

std::shared_ptr<AABB> AABB::grow(const Vector3& size) {
    return std::make_shared<AABB>(m_pos0 - size, m_pos1 + size);
}

float AABB::clipXCollide(std::shared_ptr<AABB> other, float xa) {
    if (other->getPos1().y <= m_pos0.y || other->getPos0().y >= m_pos1.y) {
        return xa;
    } else if (other->getPos1().z > m_pos0.z && other->getPos0().z < m_pos1.z) {
        if (xa > 0.f && other->getPos1().x <= m_pos0.x)
            xa = std::min(m_pos0.x - other->getPos1().x, xa);

        if (xa < 0.f && other->getPos0().x >= m_pos1.x)
            xa = std::max(xa, m_pos1.x - other->getPos0().x);

        return xa;
    } else {
        return xa;
    }
}

float AABB::clipYCollide(std::shared_ptr<AABB> other, float ya) {
    if (other->getPos1().x <= m_pos0.x || other->getPos0().x >= m_pos1.x) {
        return ya;
    } else if (other->getPos1().z > m_pos0.z && other->getPos0().z < m_pos1.z) {
        if (ya > 0.f && other->getPos1().y <= m_pos0.y)
            ya = std::min(ya, m_pos0.y - other->getPos1().y);

        if (ya < 0.f && other->getPos0().y >= m_pos1.y)
            ya = std::max(ya, m_pos1.y - other->getPos0().y);

        return ya;
    } else {
        return ya;
    }
}

float AABB::clipZCollide(std::shared_ptr<AABB> other, float za) {
    if (other->getPos1().x <= m_pos0.x || other->getPos0().x >= m_pos1.x) {
        return za;
    } else if (other->getPos1().y > m_pos0.y && other->getPos0().y < m_pos1.y) {
        if (za > 0.f && other->getPos1().z <= m_pos0.z)
            za = std::min(za, m_pos0.z - other->getPos1().z);

        if (za < 0.f && other->getPos0().z >= m_pos1.z)
            za = std::max(za, m_pos1.z - other->getPos0().z);

        return za;
    } else {
        return za;
    }
}

bool AABB::intersects(std::shared_ptr<AABB> other) {
    if (other->getPos1().x <= m_pos0.x || other->getPos0().x >= m_pos1.x) {
        return false;
    } else if (other->getPos1().y <= m_pos0.y || other->getPos0().y >= m_pos1.y) {
        return false;
    } else {
        return other->getPos1().z > m_pos0.z && other->getPos0().z < m_pos1.z;
    }
}

void AABB::move(const Vector3& other) {
    m_pos0 += other;
    m_pos1 += other;
}

Vector3 AABB::getPos0() const {
    return m_pos0;
}

Vector3 AABB::getPos1() const {
    return m_pos1;
}
