#include <Frustum.hpp>
#include <glad/glad.h>
#include <cmath>

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
        
        if (distance < -radius) return false;
    }

    return true;
}

bool Frustum::cubeInFrustum(const AABB& aabb) {
    return cubeInFrustum(aabb.min(), aabb.max());
}

// void Frustum::normalizePlane(float frustum[6][4], FrustumSide side) {
//     // Here we calculate the magnitude of the normal to the plane (point A B C)
//     // Remember that (A, B, C) is that same thing as the normal's (X, Y, Z).
//     // To calculate magnitude you use the equation: magnitude = sqrt( x^2 + y^2 + z^2)
//     float magnitude = std::sqrt(frustum[side][A] * frustum[side][A] +
//                                 frustum[side][B] * frustum[side][B] +
//                                 frustum[side][C] * frustum[side][C]);

//     // Then we divide the plane's values by its magnitude.
//     // This makes it easier to work with.
//     frustum[side][A] /= magnitude;
//     frustum[side][B] /= magnitude;
//     frustum[side][C] /= magnitude;
//     frustum[side][D] /= magnitude;
// }

// void Frustum::calculateFrustum() {
//     float proj[16];           // This will hold our projection matrix
//     float modl[16];           // This will hold our modelview matrix
//     float clip[16];           // This will hold the clipping planes

//     // glGetFloat() is used to extract information about our OpenGL world.
//     // Below, we pass in GL_PROJECTION_MATRIX to abstract our projection matrix.
//     // It then stores the matrix into an array of [16].
//     glGetFloatv(GL_PROJECTION_MATRIX, proj);

//     // By passing in GL_MODELVIEW_MATRIX, we can abstract our model view matrix.
//     // This also stores it in an array of [16].
//     glGetFloatv(GL_MODELVIEW_MATRIX, modl);

//     // Now that we have our modelview and projection matrix, if we combine these 2 matrices,
//     // it will give us our clipping planes. To combine 2 matrices, we multiply them.
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             clip[i * 4 + j] = modl[i * 4 + 0] * proj[0 + j] +
//                               modl[i * 4 + 1] * proj[4 + j] +
//                               modl[i * 4 + 2] * proj[8 + j] +
//                               modl[i * 4 + 3] * proj[12 + j];
//         }
//     }

//     // Now we actually want to get the sides of the frustum. To do this, we take
//     // the clipping planes we received above and extract the sides from them.

//     // This will extract the RIGHT side of the frustum
//     m_frustum[RIGHT][A] = clip[3] - clip[0];
//     m_frustum[RIGHT][B] = clip[7] - clip[4];
//     m_frustum[RIGHT][C] = clip[11] - clip[8];
//     m_frustum[RIGHT][D] = clip[15] - clip[12];
//     // Normalize the RIGHT side
//     normalizePlane(m_frustum, RIGHT);

//     // This will extract the LEFT side of the frustum
//     m_frustum[LEFT][A] = clip[3] + clip[0];
//     m_frustum[LEFT][B] = clip[7] + clip[4];
//     m_frustum[LEFT][C] = clip[11] + clip[8];
//     m_frustum[LEFT][D] = clip[15] + clip[12];
//     // Normalize the LEFT side
//     normalizePlane(m_frustum, LEFT);

//     // This will extract the BOTTOM side of the frustum
//     m_frustum[BOTTOM][A] = clip[3] + clip[1];
//     m_frustum[BOTTOM][B] = clip[7] + clip[5];
//     m_frustum[BOTTOM][C] = clip[11] + clip[9];
//     m_frustum[BOTTOM][D] = clip[15] + clip[13];
//     // Normalize the BOTTOM side
//     normalizePlane(m_frustum, BOTTOM);

//     // This will extract the TOP side of the frustum
//     m_frustum[TOP][A] = clip[3] - clip[1];
//     m_frustum[TOP][B] = clip[7] - clip[5];
//     m_frustum[TOP][C] = clip[11] - clip[9];
//     m_frustum[TOP][D] = clip[15] - clip[13];
//     // Normalize the TOP side
//     normalizePlane(m_frustum, TOP);

//     // This will extract the BACK side of the frustum
//     m_frustum[BACK][A] = clip[3] - clip[2];
//     m_frustum[BACK][B] = clip[7] - clip[6];
//     m_frustum[BACK][C] = clip[11] - clip[10];
//     m_frustum[BACK][D] = clip[15] - clip[14];
//     // Normalize the BACK side
//     normalizePlane(m_frustum, BACK);

//     // This will extract the FRONT side of the frustum
//     m_frustum[FRONT][A] = clip[3] + clip[2];
//     m_frustum[FRONT][B] = clip[7] + clip[6];
//     m_frustum[FRONT][C] = clip[11] + clip[10];
//     m_frustum[FRONT][D] = clip[15] + clip[14];
//     // Normalize the FRONT side
//     normalizePlane(m_frustum, FRONT);
// }

// bool Frustum::pointInFrustum(const glm::vec3& pos) {
//     // Go through all the sides of the frustum
//     for (int i = 0; i < 6; i++) {
//         // Calculate the plane equation and check if the point is behind a side of the frustum
//         if (m_frustum[i][A] * pos.x + m_frustum[i][B] * pos.y + m_frustum[i][C] * pos.z + m_frustum[i][D] <= 0) {
//             // The point was behind a side, so it ISN'T in the frustum
//             return false;
//         }
//     }

//     // The point was inside of the frustum (In front of ALL the sides of the frustum)
//     return true;
// }

// bool Frustum::sphereInFrustum(const glm::vec3& pos, float radius) {
//     // Go through all the sides of the frustum
//     for (int i = 0; i < 6; i++) {
//         // If the center of the sphere is farther away from the plane than the radius
//         if (m_frustum[i][A] * pos.x + m_frustum[i][B] * pos.y + m_frustum[i][C] * pos.z + m_frustum[i][D] <= -radius) {
//             // The distance was greater than the radius so the sphere is outside of the frustum
//             return false;
//         }
//     }

//     // The sphere was inside of the frustum!
//     return true;
// }

// bool Frustum::cubeInFrustum(const glm::vec3& min, const glm::vec3& max) {
//     // This test is a bit more work, but not too much more complicated.
//     // Basically, what is going on is, that we are given the center of the cube,
//     // and half the length.  Think of it like a radius.  Then we checking each point
//     // in the cube and seeing if it is inside the frustum.  If a point is found in front
//     // of a side, then we skip to the next side.  If we get to a plane that does NOT have
//     // a point in front of it, then it will return false.

//     // *Note* - This will sometimes say that a cube is inside the frustum when it isn't.
//     // This happens when all the corners of the bounding box are not behind any one plane.
//     // This is rare and shouldn't effect the overall rendering speed.

//     for (int i = 0; i < 6; i++) {
//         if (m_frustum[i][A] * min.x + m_frustum[i][B] * min.y + m_frustum[i][C] * min.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * max.x + m_frustum[i][B] * min.y + m_frustum[i][C] * min.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * min.x + m_frustum[i][B] * max.y + m_frustum[i][C] * min.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * max.x + m_frustum[i][B] * max.y + m_frustum[i][C] * min.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * min.x + m_frustum[i][B] * min.y + m_frustum[i][C] * max.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * max.x + m_frustum[i][B] * min.y + m_frustum[i][C] * max.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * min.x + m_frustum[i][B] * max.y + m_frustum[i][C] * max.z + m_frustum[i][D] > 0)
//             continue;
//         if (m_frustum[i][A] * max.x + m_frustum[i][B] * max.y + m_frustum[i][C] * max.z + m_frustum[i][D] > 0)
//             continue;

//         // If we get here, it isn't in the frustum
//         return false;
//     }

//     return true;
// }