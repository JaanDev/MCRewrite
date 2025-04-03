#include <Player.hpp>
#include <Game.hpp>
#include <algorithm>
#include <impl/Math.hpp>

Player::Player(Level& level)
    : m_level(level), m_pos(0.f), m_prevPos(0.f), m_aabb(), m_rot(0.f), m_motion(0.f), m_onGround(false) { resetPos(); }

void Player::resetPos() {
    setPos({
        Math::random() * m_level.getWidth(), 
        m_level.getDepth() + 3, 
        Math::random() * m_level.getHeight()
    });
}
void Player::setPos(const glm::vec3& pos) {
    const float w = .3f;
    const float h = .9f;
    
    m_pos = pos;
    m_aabb = AABB(m_pos - glm::vec3(w, h, w), m_pos + glm::vec3(w, h, w));
}

void Player::turn(const glm::vec2& delta) {
    m_rot += delta * 0.15f;
    m_rot.y = std::clamp(m_rot.y, -89.9f, 89.9f); // using 89.9 feels like a cheat but im too lazy to make it better =)
}

void Player::tick() {
    m_prevPos = m_pos;
    float forward, vertical;
    forward = vertical = 0.f;

    auto window = Game::get().getWindow();

    if (glfwGetKey(window, GLFW_KEY_R)) {
        resetPos();
    }

    if (glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W)) {
        forward += 1.f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S)) {
        forward -= 1.f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A)) {
        vertical -= 1.f;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D)) {
        vertical += 1.f;
    }

    if ((glfwGetKey(window, GLFW_KEY_SPACE) || glfwGetKey(window, GLFW_KEY_LEFT_SUPER)) && m_onGround)
        m_motion.y = .12f;

    moveRelative(forward, vertical, m_onGround ? .02f : .005f);

    m_motion.y -= .005f;
    
    move(m_motion);
    
    m_motion *= glm::vec3(0.91f, 0.98f, 0.91f);

    if (m_onGround) {
        m_motion.x *= 0.8f;
        m_motion.z *= 0.8f;
    }
}

void Player::move(const glm::vec3& delta) {
    auto org = delta;
    auto a = delta;
    auto aabbs = m_level.getCubes(m_aabb.expand(delta));

    for (auto& aabb : aabbs) {
        a.x = aabb.clipXCollide(m_aabb, a.x);
    }
    m_aabb.move(glm::vec3(a.x, 0.f, 0.f));

    for (auto& aabb : aabbs) {
        a.y = aabb.clipYCollide(m_aabb, a.y);
    }
    m_aabb.move(glm::vec3(0.f, a.y, 0.f));

    for (auto& aabb : aabbs) {
        a.z = aabb.clipZCollide(m_aabb, a.z);
    }
    m_aabb.move(glm::vec3(0.f, 0.f, a.z));

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

    m_pos = {
        (m_aabb.min().x + m_aabb.max().x) / 2.f, 
        m_aabb.min().y + 1.62f,
        (m_aabb.min().z + m_aabb.max().z) / 2.f
    };
}

void Player::moveRelative(float xa, float za, float speed) {
    glm::vec2 input(xa, za);

    if (glm::length(input) < 0.1f) return;

    input = glm::normalize(input) * speed;

    float yawRad = glm::radians(m_rot.x);
    float cosYaw = cos(yawRad);
    float sinYaw = sin(yawRad);

    m_motion.x += input.x * cosYaw - input.y * sinYaw;
    m_motion.z += input.y * cosYaw + input.x * sinYaw;
}