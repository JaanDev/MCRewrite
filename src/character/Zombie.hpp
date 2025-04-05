#pragma once
#include <Entity.hpp>
#include <character/Cube.hpp>
#include <Level.hpp>

class Zombie : public Entity {
public:
    Zombie(Level& level, const glm::vec3& pos);
    // void tick() override;
    void render();

protected:
    Cube m_head;
    Cube m_body;
    Cube m_arm0;
    Cube m_arm1;
    Cube m_leg0;
    Cube m_leg1;

    float m_rot;
    float m_rotA;
    float m_timeOff;
    float m_speed;
};