#pragma once
#include <AABB.hpp>
#include <vector>

enum FrustumSide {
    RIGHT = 0,
    LEFT = 1,
    BOTTOM = 2,
    TOP = 3,
    BACK = 4,
    FRONT = 5
};

enum FrustumPlane {
    A = 0,
    B = 1,
    C = 2,
    D = 3
};

class Frustum {
private:
    float m_frustum[6][4];
    std::vector<float> m_modl_b;
    std::vector<float> m_proj_b;

public:
    Frustum();

    static Frustum* get() {
        static auto frustum = new Frustum();
        frustum->calculateFrustum();

        return frustum;
    }

    void normalizePlane(float frustum[6][4], FrustumSide side);
    void calculateFrustum();
    bool pointInFrustum(const glm::vec3& pos);
    bool sphereInFrustum(const glm::vec3& pos, float radius);
    bool cubeInFrustum(const glm::vec3& min, const glm::vec3& max);
    bool cubeInFrustum(const AABB& aabb);
};