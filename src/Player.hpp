#pragma once
#include "includes.hpp"
#include "Level.hpp"
#include "AABB.hpp"

class Player {
  public:
    Player(std::shared_ptr<Level> level);
    // random pos
    void resetPos();
    void setPos(const Vector3& pos);
    void turn(const Vector2& delta);
    void tick();
    void move(const Vector3& delta);
    void moveRelative(float xa, float za, float speed);
    Vector2 getRot();
    Vector3 getPosO();
    Vector3 getPos();

  private:
    std::shared_ptr<Level> m_level;
    Vector3 m_pos;
    std::shared_ptr<AABB> m_aabb;
    Vector2 m_rot;
    Vector3 m_moveDelta;
    bool m_onGround;
    Vector3 m_posO;
};