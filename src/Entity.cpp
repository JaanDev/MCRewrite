#include <Entity.hpp>
#include <algorithm>
#include <impl/Math.hpp>
#include <impl/InputHelper.hpp>

Entity::Entity(Level& level) : m_level(level), m_pos(0.f), m_poso(0.f), m_bb(), m_rot(0.f), m_motion(0.f), m_onGround(false), m_heightOffset(0.f) {
    resetPos();
}

void Entity::resetPos() {
    setPos({Math::random() * m_level.getWidth(), m_level.getDepth() + 10, Math::random() * m_level.getHeight()});
}

void Entity::setPos(const glm::vec3& pos) {
    const float w = .3f;
    const float h = .9f;

    m_pos = pos;
    m_bb = AABB(m_pos - glm::vec3(w, h, w), m_pos + glm::vec3(w, h, w));
}

void Entity::turn(const glm::vec2& delta) {
    m_rot += delta * 0.15f;
    m_rot.y = std::clamp(m_rot.y, -89.9f, 89.9f); // using 89.9 feels like a cheat but im too lazy to make it better =)
}

void Entity::tick() {
    m_poso = m_pos;
}

void Entity::move(const glm::vec3& delta) {
    auto org = delta;
    auto a = delta;
    auto aabbs = m_level.getCubes(m_bb.expand(delta));

    for (auto& aabb : aabbs) {
        a.x = aabb.clipXCollide(m_bb, a.x);
    }
    m_bb.move(glm::vec3(a.x, 0.f, 0.f));

    for (auto& aabb : aabbs) {
        a.y = aabb.clipYCollide(m_bb, a.y);
    }
    m_bb.move(glm::vec3(0.f, a.y, 0.f));

    for (auto& aabb : aabbs) {
        a.z = aabb.clipZCollide(m_bb, a.z);
    }
    m_bb.move(glm::vec3(0.f, 0.f, a.z));

    m_onGround = org.y != a.y && org.y < 0.f;

    if (org.x != a.x) {
        m_motion.x = 0.f;
    }

    if (org.y != a.y) {
        m_motion.y = 0.f;
    }

    if (org.z != a.z) {
        m_motion.z = 0.f;
    }

    m_pos = {(m_bb.min().x + m_bb.max().x) / 2.f, m_bb.min().y + m_heightOffset, (m_bb.min().z + m_bb.max().z) / 2.f};
}

void Entity::moveRelative(float xa, float za, float speed) {
    glm::vec2 input(xa, za);

    if (glm::length(input) < 0.1f) {
        return;
    }

    input = glm::normalize(input) * speed;

    float yawRad = glm::radians(m_rot.x);
    float cosYaw = cos(yawRad);
    float sinYaw = sin(yawRad);

    m_motion.x += input.x * cosYaw - input.y * sinYaw;
    m_motion.z += input.y * cosYaw + input.x * sinYaw;
}