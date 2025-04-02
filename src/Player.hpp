#pragma once
#include <Level.hpp>
#include <AABB.hpp>

class Player {
public:
    Player(Level& level);

    void tick();

    void resetPos();
    void setPos(const glm::vec3& pos);
    void turn(const glm::vec2& delta);
    void move(const glm::vec3& delta);
    void moveRelative(float xa, float za, float speed);
    inline const glm::vec2& getRot() const { return m_rot; }
    inline const glm::vec3& getPos() const { return m_pos; } 
    inline const glm::vec3& getPrevPos() const { return m_prevPos; } 
    AABB getAABB() { return m_aabb; }

private:
    Level& m_level;
    AABB m_aabb;
    glm::vec3 m_pos;
    glm::vec3 m_prevPos;
    glm::vec2 m_rot;
    glm::vec3 m_motion;
    bool m_onGround;
};