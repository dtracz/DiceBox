#include "utils/lever_calculators.h"
#include "base/colors.h"
#include "base/geometry.h"
#include "base/parts.hpp"
#include "components/Cylinder.h"
#include "components/Square.h"
#include "core/Component2D.h"
#include "core/Union.h"
#include "globals.h"
#include <cmath>
#include <iterator>
#include <numbers>
#include <numeric>
#include <utility>
#include <vector>



Component nail(
    Vec3 position = Vec3::ZERO(),
    Color color = { 1, 0, 0 },
    double r = 1,
    double h = 8
)
{
    auto cylinder = Cylinder::create(r, r, h);
    cylinder.translate(position.x, position.y, position.z);
    cylinder.color(color.x, color.y, color.z, color.alpha);
    return cylinder;
}


Vec2 LinearLeverCalculator::cover_position(double open) const
{
    double angle = open * _range;
    Vec2 curr_upper_mp = rotate2D(_lower_mp, _upper_mp0, angle);
    return curr_upper_mp - _upper_mp0;
}


double
NonLinearLeverCalculator::get_upper_lever_angle(double open, bool radians) const
{
    double offset = angle2D(_upper_mp0 - _upper_axis);
    double range = angle2D(_upper_mp1 - _upper_axis) - offset;
    double angle = offset + open * range;
    return radians ? angle : angle * 360 / (2 * std::numbers::pi);
}


double
NonLinearLeverCalculator::get_lower_lever_angle(double open, bool radians) const
{
    double upper_angle = get_upper_lever_angle(open, true);
    Vec2 curr_upper_mp = _upper_axis + Vec2(upper_lever_length(), 0);
    curr_upper_mp = rotate2D(_upper_axis, curr_upper_mp, upper_angle);
    auto intersects = _circles_cross(
        curr_upper_mp, cover_mp_distance(), _lower_axis, lower_lever_length()
    );
    double angle = angle2D(intersects.second - _lower_axis);
    return radians ? angle : angle * 360 / (2 * std::numbers::pi);
}


double
NonLinearLeverCalculator::get_cover_angle(double open, bool radians) const
{
    double upper_angle = get_upper_lever_angle(open, true);
    Vec2 curr_upper_mp = _upper_axis + Vec2(upper_lever_length(), 0);
    curr_upper_mp = rotate2D(_upper_axis, curr_upper_mp, upper_angle);
    Vec2 curr_lower_mp = _lower_mp + get_cover_shift(open);
    double angle0 = angle2D(_lower_mp - _upper_mp0);
    double angle = angle2D(curr_lower_mp - curr_upper_mp);
    angle = std::fmod(
        angle - angle0 + 2 * std::numbers::pi, 2 * std::numbers::pi
    );
    return radians ? angle : angle * 360 / (2 * std::numbers::pi);
}


Vec2 NonLinearLeverCalculator::get_cover_shift(double open) const
{
    double lower_angle = get_lower_lever_angle(open, true);
    Vec2 curr_lower_mp = _lower_axis + Vec2(lower_lever_length(), 0);
    curr_lower_mp = rotate2D(_lower_axis, curr_lower_mp, lower_angle);
    return curr_lower_mp - _lower_mp;
}


Module3D NonLinearLeverCalculator::get_visualisation(
    double open, double height, double pin_r, double pin_h
) const
{
    Vec3 veps = { 0, 0, 0.001 };
    auto upper_lever_wheel = _pinned_wheel_with_arc(
        upper_lever_length(), height, pin_r, pin_h, { 0, 1, 0, 0.4 }
    );
    upper_lever_wheel.translate(Vec3::fromXY(_upper_axis) + veps * 2);
    auto moving_wheel = _pinned_wheel_with_arc(
        cover_mp_distance(), height, pin_r, pin_h, { 1, 0, 1, 0.4 }
    );
    double upper_angle = get_upper_lever_angle(open, true);
    Vec2 curr_upper_mp = _upper_axis + Vec2(upper_lever_length(), 0);
    curr_upper_mp = rotate2D(_upper_axis, curr_upper_mp, upper_angle);
    moving_wheel.translate(Vec3::fromXY(curr_upper_mp));
    auto lower_lever_wheel = _pinned_wheel_with_arc(
        lower_lever_length(), height, pin_r, pin_h, { 0, 0, 1, 0.4 }
    );
    lower_lever_wheel.translate(Vec3::fromXY(_lower_axis) + veps);
    double lower_angle = get_lower_lever_angle(open, true);
    Vec2 curr_lower_mp = _lower_axis + Vec2(lower_lever_length(), 0);
    curr_lower_mp = rotate2D(_lower_axis, curr_lower_mp, lower_angle);
    auto n0 = nail(Vec3::fromXY(curr_lower_mp), { 1, 1, 1 });
    auto n1 = nail(Vec3::fromXY(_upper_axis));
    auto n2 = nail(Vec3::fromXY(_upper_mp0));
    auto n3 = nail(Vec3::fromXY(_upper_mp1));
    auto n4 = nail(Vec3::fromXY(_lower_axis));
    auto n5 = nail(Vec3::fromXY(_lower_mp));
    HelperPart nails = n0 + n1 + n2 + n3 + n4 + n5;
    return nails + lower_lever_wheel + upper_lever_wheel + moving_wheel;
}


std::pair<Vec2, Vec2> NonLinearLeverCalculator::_circles_cross(
    Vec2 center1, double r1, Vec2 center2, double r2
)
{
    double centers_dist = (center2 - center1).length();
    if (centers_dist > r1 + r2 || centers_dist < std::abs(r2 - r1))
        throw std::runtime_error("given circles dont intersect");
    double cos_theta = centers_dist * centers_dist + r1 * r1 - r2 * r2;
    cos_theta /= 2 * centers_dist * r1;
    double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
    Vec2 vp = (center2 - center1) * (r1 / centers_dist);
    Vec2 intersect1 = { vp.x * cos_theta - vp.y * sin_theta + center1.x,
                        vp.x * sin_theta + vp.y * cos_theta + center1.y };
    Vec2 intersect2 = { vp.x * cos_theta + vp.y * sin_theta + center1.x,
                        -vp.x * sin_theta + vp.y * cos_theta + center1.y };
    return { intersect1, intersect2 };
}


HelperPart NonLinearLeverCalculator::_pinned_wheel_with_arc(
    double r, double h, double pin_r, double pin_h, Color color
)
{
    auto wheel = Cylinder::create(r, r, h);
    wheel.color(color.x, color.y, color.z, color.alpha);
    color.alpha = 1;
    auto center = nail(Vec3::ZERO(), color, pin_r, pin_h);
    center.color(color.x, color.y, color.z, color.alpha);
    return center + wheel;
}
