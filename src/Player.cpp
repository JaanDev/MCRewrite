#include "Player.hpp"

Player::Player(std::shared_ptr<Level> level)
    : m_level(level), m_pos({0, 0, 0}), m_aabb(nullptr), m_rot({0, 0}), m_moveDelta({0, 0, 0}), m_onGround(false), m_posO({0, 0, 0}) {
    resetPos();
}

void Player::resetPos() {
    // setPos({randomFloat(0.f, chunksCount * chunkSize), chunkHeight + 10.f, randomFloat(0.f, chunksCount * chunkSize)});
    setPos({randomFloat(0.f, chunksCount * chunkSize), surfaceLevel + 10.f, randomFloat(0.f, chunksCount * chunkSize)});
}

void Player::setPos(const Vector3& pos) {
    m_pos = pos;
    const float w = .3f;
    const float h = .9f;
    m_aabb = std::make_shared<AABB>(m_pos - Vector3 {w, h, w}, m_pos + Vector3 {w, h, w});
}

void Player::turn(const Vector2& delta) {
    m_rot += delta * mouseSensitivity;

    m_rot.y = std::clamp(m_rot.y, -89.9f * DEG2RAD, 89.9f * DEG2RAD); // using 89.9 feels like a cheat but im too lazy to make it better =)
}

void Player::tick() {
    m_posO = m_pos;
    float xa, ya;
    xa = ya = 0.f;

    if (IsKeyDown(KEY_R))
        resetPos();

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        ya += 1.f;

    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        ya -= 1.f;

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        xa += 1.f;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        xa -= 1.f;

    if ((IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_LEFT_SUPER)) && m_onGround)
        m_moveDelta.y = .12f;

    moveRelative(xa, ya, m_onGround ? .02f : .005f);
    m_moveDelta.y -= .005f;
    move(m_moveDelta);
    m_moveDelta.x *= 0.91f;
    m_moveDelta.y *= 0.98f;
    m_moveDelta.z *= 0.91f;
    if (m_onGround) {
        m_moveDelta.x *= 0.8f;
        m_moveDelta.z *= 0.8f;
    }
}

void Player::move(const Vector3& delta) {
    Vector3 org = delta;
    Vector3 a = delta;
    auto aabbs = m_level->getCubes(m_aabb->expand(delta));

    for (const auto& aabb : aabbs) {
        a.x = aabb->clipXCollide(m_aabb, a.x);
        a.y = aabb->clipYCollide(m_aabb, a.y);
        a.z = aabb->clipZCollide(m_aabb, a.z);
    }

    m_aabb->move(a);

    m_onGround = org.y != a.y && org.y < 0.f;

    if (org.x != a.x)
        m_moveDelta.x = 0.f;

    if (org.y != a.y)
        m_moveDelta.y = 0.f;

    if (org.z != a.z)
        m_moveDelta.z = 0.f;

    m_pos = {(m_aabb->getPos0().x + m_aabb->getPos1().x) / 2.f, m_aabb->getPos0().y + 1.62f,
             (m_aabb->getPos0().z + m_aabb->getPos1().z) / 2.f};
}

void Player::moveRelative(float xa, float za, float speed) {
    auto dist = xa * xa + za * za;
    if (dist >= .01f) {
        dist = speed / sqrtf(dist);
        xa *= dist;
        za *= dist;
        auto sin = sinf(m_rot.x);
        auto cos = cosf(m_rot.x);
        m_moveDelta.x += xa * cos - za * sin;
        m_moveDelta.z += za * cos + xa * sin;
    }
}

Vector2 Player::getRot() {
    return m_rot;
}

Vector3 Player::getPosO() {
    return m_posO;
}

Vector3 Player::getPos() {
    return m_pos;
}

std::shared_ptr<AABB> Player::getAABB() {
    return m_aabb;
}
