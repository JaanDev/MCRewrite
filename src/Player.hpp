#pragma once
#include <Level.hpp>
#include <AABB.hpp>

class Player {
public:
    Player(Level& level);

    void tick();

    void resetPos();
    void setPos(glm::vec3 pos);
    void turn(glm::vec2 delta);
    void move(glm::vec3 delta);
    void moveRelative(float xa, float za, float speed);
    glm::vec2 getRot() const { return m_rot; }
    glm::vec3 getPos() const { return m_pos; } 
    AABB getAABB() { return m_aabb; }

private:
    Level& m_level;
    AABB& m_aabb;
    glm::vec3 m_pos;
    glm::vec2 m_rot;
    glm::vec3 m_motion;
    bool m_onGround;
};