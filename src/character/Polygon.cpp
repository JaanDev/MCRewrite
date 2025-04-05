#include <character/Polygon.hpp>

Polygon::Polygon(const std::array<Vertex, 4>& vertices) : m_vertices(vertices) {}

Polygon::Polygon(const std::array<Vertex, 4>& vertices, int u0, int v0, int u1, int v1) : Polygon(vertices) {
    m_vertices[0].remap(u1, v0);
    m_vertices[1].remap(u0, v0);
    m_vertices[2].remap(u0, v1);
    m_vertices[3].remap(u1, v1);
}