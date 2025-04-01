#pragma once
#include <AABB.hpp>
#include <vector>

enum FrustumSide {
    LEFT = 0,
    RIGHT = 1,
    BOTTOM = 2,
    TOP = 3,
    BACK = 4,
    FRONT = 5
};

struct FrustumPlane {
    glm::vec3 normal;
    float distance;
};

class Frustum {
private:
    // float m_frustum[6][4];
    // std::vector<float> m_modl_b;
    // std::vector<float> m_proj_b;
    std::vector<FrustumPlane> m_planes;

public:
    Frustum();

    static Frustum* get() {
        static auto frustum = new Frustum();
        return frustum;
    }

    void calculateFrustum(const glm::mat4& mvp);

    // void normalizePlane(float frustum[6][4], FrustumSide side);
    // void calculateFrustum();
    // bool pointInFrustum(const glm::vec3& pos);
    // bool sphereInFrustum(const glm::vec3& pos, float radius);
    bool cubeInFrustum(const glm::vec3& min, const glm::vec3& max);
    bool cubeInFrustum(const AABB& aabb);
};