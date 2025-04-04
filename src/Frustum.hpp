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
    std::vector<FrustumPlane> m_planes;

public:
    Frustum();

    static Frustum* get() {
        static auto frustum = new Frustum();
        return frustum;
    }

    void calculateFrustum(const glm::mat4& mvp);

    bool cubeInFrustum(const glm::vec3& min, const glm::vec3& max);
    bool cubeInFrustum(const AABB& aabb);
};