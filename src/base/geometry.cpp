#include "base/geometry.h"
#include <cmath>

Vec2 rotate2D(Vec2 axis, Vec2 point, double angle)
{
    return axis
         + Vec2(
               (point.x - axis.x) * std::cos(angle)
                   - (point.y - axis.y) * std::sin(angle),
               (point.x - axis.x) * std::sin(angle)
                   + (point.y - axis.y) * std::cos(angle)
         );
}

Vec2 mirror(Vec2 point, Vec2 mirror_plane)
{
    // represent mirror_plane as parallel vector, instead of perpendicular
    mirror_plane = { mirror_plane.y, -mirror_plane.x };
    double projection_length = mirror_plane.dot(point)
                             / mirror_plane.dot(mirror_plane);
    Vec2 point_projection = mirror_plane * projection_length;
    return (point_projection * 2) - point;
}
