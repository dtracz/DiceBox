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
