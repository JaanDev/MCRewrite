#pragma once
#include <AABB.hpp>
#include <Level.hpp>

class Entity {
public:
    Entity(Level& level);

    virtual void tick();

    void resetPos();
    void setPos(const glm::vec3& pos);
    void turn(const glm::vec2& delta);
    void move(const glm::vec3& delta);
    void moveRelative(float xa, float za, float speed);
    inline const glm::vec2& getRot() const { return m_rot; }
    inline const glm::vec3& getPos() const { return m_pos; }
    inline const glm::vec3& getPosO() const { return m_poso; }
    inline const AABB& getAABB() const { return m_bb; }

protected:
    Level& m_level;
    AABB m_bb;
    glm::vec3 m_pos;
    glm::vec3 m_poso;
    glm::vec2 m_rot;
    glm::vec3 m_motion;
    bool m_onGround;
    float m_heightOffset;
};