#include <Player.hpp>
#include <Game.hpp>
#include <algorithm>
#include <impl/Math.hpp>
#include <impl/InputHelper.hpp>

Player::Player(Level& level) : Entity::Entity(level) {
    m_heightOffset = 1.62f;
}

void Player::tick() {
    Entity::tick();

    float forward, vertical;
    forward = vertical = 0.f;

    if (InputHelper::isKeyDown(GLFW_KEY_R)) {
        resetPos();
    }

    if (InputHelper::isKeyDown(GLFW_KEY_UP) || InputHelper::isKeyDown(GLFW_KEY_W)) {
        forward += 1.f;
    }

    if (InputHelper::isKeyDown(GLFW_KEY_DOWN) || InputHelper::isKeyDown(GLFW_KEY_S)) {
        forward -= 1.f;
    }

    if (InputHelper::isKeyDown(GLFW_KEY_LEFT) || InputHelper::isKeyDown(GLFW_KEY_A)) {
        vertical -= 1.f;
    }

    if (InputHelper::isKeyDown(GLFW_KEY_RIGHT) || InputHelper::isKeyDown(GLFW_KEY_D)) {
        vertical += 1.f;
    }

    if ((InputHelper::isKeyDown(GLFW_KEY_SPACE) || InputHelper::isKeyDown(GLFW_KEY_LEFT_SUPER)) && m_onGround) {
        m_motion.y = .12f;
    }

    moveRelative(forward, vertical, m_onGround ? .02f : .005f);

    m_motion.y -= .005f;

    move(m_motion);

    m_motion *= glm::vec3(0.91f, 0.98f, 0.91f);

    if (m_onGround) {
        m_motion.x *= 0.8f;
        m_motion.z *= 0.8f;
    }
}