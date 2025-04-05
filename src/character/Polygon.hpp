#pragma once
#include <array>
#include <vector>
#include <utils.hpp>

class Polygon {
public:
    Polygon(const std::array<Vertex, 4>& vertices);
    Polygon(const std::array<Vertex, 4>& vertices, int u0, int v0, int u1, int v1);
    
private:
    std::array<Vertex, 4> m_vertices;
};