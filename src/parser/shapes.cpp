#include "parser/shapes.h"
#include "base/geometry.h"
#include <cmath>
#include <numbers>

using namespace parser;

void SimpleShape::mirror(Vec2 mirror_plane)
{
    double plane_angle = angle2D(mirror_plane);
    rotation = std::numbers::pi + 2 * plane_angle - rotation;
    Vec2 dist_to_mirror = mirror_plane * position.dot(mirror_plane);
    Vec2 length_along_mirror = position - dist_to_mirror;
    position = -dist_to_mirror + length_along_mirror;
}


void Rectangle::mirror(Vec2 mirror_plane)
{
    position = ::mirror(position, mirror_plane);
    Vec2 real_diagonal = rotate2D(size, rotation);
    real_diagonal = ::mirror(real_diagonal, mirror_plane);
    rotation = 2 * angle2D(mirror_plane) - rotation;
    size = rotate2D(real_diagonal, -rotation);
    _normalize();
}

void Rectangle::_normalize()
{
    if (size.x < 0) {
        position += rotate2D({ size.x, 0 }, rotation);
        size.x = -size.x;
    }
    if (size.y < 0) {
        position += rotate2D({ 0, size.y }, rotation);
        size.y = -size.y;
    }
}


void Polygon::mirror(Vec2 mirror_plane)
{
    position = ::mirror(position, mirror_plane);
    rotation = 2 * angle2D(mirror_plane) - rotation;
    for (Vec2& vertex : vertices)
        vertex = ::mirror(vertex, mirror_plane);
}
