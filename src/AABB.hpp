#pragma once
#include "includes.hpp"

class AABB {
  public:
    AABB(const Vector3& pos0, const Vector3& pos1);
    std::shared_ptr<AABB> expand(const Vector3& size);
    std::shared_ptr<AABB> grow(const Vector3& size);
    float clipXCollide(std::shared_ptr<AABB> other, float xa);
    float clipYCollide(std::shared_ptr<AABB> other, float ya);
    float clipZCollide(std::shared_ptr<AABB> other, float za);
    bool intersects(std::shared_ptr<AABB> other);
    void move(const Vector3& other);

    Vector3 getPos0() const;
    Vector3 getPos1() const;

  private:
    Vector3 m_pos0;
    Vector3 m_pos1;
};