#include <Frustum.hpp>
#include <glad/glad.h>

Frustum::Frustum() : m_planes(6) {}

void Frustum::calculateFrustum(const glm::mat4& mvp) {
    m_planes[LEFT].normal.x = mvp[0][3] + mvp[0][0];
    m_planes[LEFT].normal.y = mvp[1][3] + mvp[1][0];
    m_planes[LEFT].normal.z = mvp[2][3] + mvp[2][0];
    m_planes[LEFT].distance = mvp[3][3] + mvp[3][0];

    m_planes[RIGHT].normal.x = mvp[0][3] - mvp[0][0];
    m_planes[RIGHT].normal.y = mvp[1][3] - mvp[1][0];
    m_planes[RIGHT].normal.z = mvp[2][3] - mvp[2][0];
    m_planes[RIGHT].distance = mvp[3][3] - mvp[3][0];

    m_planes[BOTTOM].normal.x = mvp[0][3] + mvp[0][1];
    m_planes[BOTTOM].normal.y = mvp[1][3] + mvp[1][1];
    m_planes[BOTTOM].normal.z = mvp[2][3] + mvp[2][1];
    m_planes[BOTTOM].distance = mvp[3][3] + mvp[3][1];

    m_planes[TOP].normal.x = mvp[0][3] - mvp[0][1];
    m_planes[TOP].normal.y = mvp[1][3] - mvp[1][1];
    m_planes[TOP].normal.z = mvp[2][3] - mvp[2][1];
    m_planes[TOP].distance = mvp[3][3] - mvp[3][1];

    m_planes[BACK].normal.x = mvp[0][3] + mvp[0][2];
    m_planes[BACK].normal.y = mvp[1][3] + mvp[1][2];
    m_planes[BACK].normal.z = mvp[2][3] + mvp[2][2];
    m_planes[BACK].distance = mvp[3][3] + mvp[3][2];

    m_planes[FRONT].normal.x = mvp[0][3] - mvp[0][2];
    m_planes[FRONT].normal.y = mvp[1][3] - mvp[1][2];
    m_planes[FRONT].normal.z = mvp[2][3] - mvp[2][2];
    m_planes[FRONT].distance = mvp[3][3] - mvp[3][2];

    // Normalize planes
    for (auto& plane : m_planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }
}

bool Frustum::cubeInFrustum(const glm::vec3& min, const glm::vec3& max) {
    glm::vec3 center = (min + max) * 0.5f;
    glm::vec3 extents = max - center;

    for (const auto& plane : m_planes) {
        float radius = glm::dot(extents, glm::abs(plane.normal));
        float distance = glm::dot(plane.normal, center) + plane.distance;

        if (distance < -radius) {
            return false;
        }
    }

    return true;
}

bool Frustum::cubeInFrustum(const AABB& aabb) {
    return cubeInFrustum(aabb.min(), aabb.max());
}
