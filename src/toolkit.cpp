#include <utility>
#include <vector>
#include <numeric>
#include <iterator>
#include <cmath>
#include <numbers>
#include "core/Component2D.h"
#include "components/Square.h"
#include "components/Cylinder.h"
#include "core/Union.h"
#include "toolkit.h"
#include "base.h"
#include "globals.h"



Component2D crenels(std::pair<double, double> crenel_dims,
                    double length, double offset) {
    double x = crenel_dims.first;
    double y = crenel_dims.second;
    std::vector<Component2D> crenels_v;
    while (offset < length) {
        double real_x = offset + x < length
                      ? x + std::min(offset, .0)
                      : length - offset;
        if (real_x <= 0)
            continue;
        crenels_v.push_back(
            Square::create(real_x, y, false).translate(std::max(offset, .0), 0, 0)
        );
        offset += 2*x;
    }
    if (crenels_v.empty())
        throw std::runtime_error("no crenels created");
    return std::accumulate(
            std::next(crenels_v.begin()),
            crenels_v.end(),
            crenels_v.front()
    );
}


Component2D crenellated_wall(std::pair<double, double> dims,
                             Vec3 top_crenel_params,
                             Vec3 bottom_crenel_params,
                             Vec3 left_crenel_params,
                             Vec3 right_crenel_params) {
    double x = dims.first - left_crenel_params.y - right_crenel_params.y;
    double y = dims.second - top_crenel_params.y - bottom_crenel_params.y;
    auto wall = Square::create(x, y, false);
    wall.translate(left_crenel_params.y, bottom_crenel_params.y, 0);
    if (top_crenel_params.y > 0) {
        auto top_crenels = crenels(
                {top_crenel_params.x, top_crenel_params.y},
                x, top_crenel_params.z
        );
        top_crenels.translate(left_crenel_params.y, dims.second - top_crenel_params.y, 0);
        wall = wall + top_crenels;
    }
    if (bottom_crenel_params.y > 0) {
        auto bottom_crenels = crenels(
                {bottom_crenel_params.x, bottom_crenel_params.y},
                x, bottom_crenel_params.z
        );
        bottom_crenels.translate(left_crenel_params.y, 0, 0);
        wall = wall + bottom_crenels;
    }
    if (left_crenel_params.y > 0) {
        auto left_crenels = crenels(
                {left_crenel_params.x, left_crenel_params.y},
                dims.second, left_crenel_params.z
        );
        left_crenels.rotate(0, 0, -90);
        left_crenels.translate(0, dims.second, 0);
        wall = wall + left_crenels;
    }
    if (right_crenel_params.y > 0) {
        auto right_crenels = crenels(
                {right_crenel_params.x, right_crenel_params.y},
                dims.second, right_crenel_params.z);
        right_crenels.rotate(0, 0, -90);
        right_crenels.translate(dims.first - right_crenel_params.y, dims.second, 0);
        wall = wall + right_crenels;
    }
    return wall;
}




Color DistinguishableColorGenerator::next() {
    _offset++;
    return this->operator[](0);
}


Color DistinguishableColorGenerator::operator[](size_t idx) const {
    idx += _offset;
    double value = _get_value(idx / 6);
    Color color = _basic_colors[idx % 6];
    color *= value;
    return color;
}


double DistinguishableColorGenerator::_get_value(size_t idx) {
    if (idx == 0)
        return 1;
    double denom = std::exp2(1 + std::floor(std::log2(idx)));
    double numer = idx - denom/2;
    numer = 2*numer + 1;
    return numer/denom;
}


const Color DistinguishableColorGenerator::_basic_colors[] = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,1,0},
        {1,0,1},
        {0,1,1}
};


Component nail(
        Vec3 position=Vec3::ZERO(), Color color={1,0,0},
        double r=1, double h=8
) {
    auto cylinder = Cylinder::create(r, r, h);
    cylinder.translate(position.x, position.y, position.z);
    cylinder.color(color.x, color.y, color.z, color.alpha);
    return cylinder;
}


double NonLinearLeverCalculator::get_upper_lever_angle(
        double open,
        bool radians
) const {
    Vec3 ax = _vec3(_upper_axis);
    Vec3 mp0 = _vec3(_upper_mp0);
    Vec3 mp1 = _vec3(_upper_mp1);
    double offset = _angle2D(mp0 - ax);
    double range = _angle2D(mp1 - ax) - offset;
    double angle = offset + open*range;
    return radians ? angle : angle * 360 / (2*std::numbers::pi);
}


double NonLinearLeverCalculator::get_lower_lever_angle(
        double open,
        bool radians
) const {
    double upper_angle = get_upper_lever_angle(open, true);
    Vec3 upper_ax = _vec3(_upper_axis);
    Vec3 upper_mp = upper_ax + Vec3(upper_lever_length(), 0, 0);
    upper_mp = _rotate2D(upper_ax, upper_mp, upper_angle);
    Vec3 lower_ax = _vec3(_lower_axis);
    auto intersects = _circles_cross(
            upper_mp, cover_mp_distance(),
            lower_ax, lower_lever_length()
    );
    double angle = _angle2D(intersects.second - lower_ax);
    return radians ? angle : angle * 360 / (2*std::numbers::pi);
}


double NonLinearLeverCalculator::get_cover_angle(
        double open,
        bool radians
) const {
    double upper_angle = get_upper_lever_angle(open, true);
    Vec3 upper_ax = _vec3(_upper_axis);
    Vec3 upper_mp = upper_ax + Vec3(upper_lever_length(), 0, 0);
    upper_mp = _rotate2D(upper_ax, upper_mp, upper_angle);
    Vec3 lower_mp = _vec3(_lower_mp) + _vec3(get_cover_shift(open));
    double angle0 = _angle2D(_vec3(_lower_mp) - _vec3(_upper_mp0));
    double angle = _angle2D(lower_mp - upper_mp);
    angle = std::fmod(angle - angle0 + 2*std::numbers::pi, 2*std::numbers::pi);
    return radians ? angle : angle * 360 / (2*std::numbers::pi);
}


std::pair<double, double> NonLinearLeverCalculator::get_cover_shift(
        double open
) const {
    double lower_angle = get_lower_lever_angle(open, true);
    Vec3 lower_ax = _vec3(_lower_axis);
    Vec3 lower_mp = lower_ax + Vec3(lower_lever_length(), 0, 0);
    lower_mp = _rotate2D(lower_ax, lower_mp, lower_angle);
    return {lower_mp.x - _lower_mp.first, lower_mp.y - _lower_mp.second};
}


Module3D NonLinearLeverCalculator::get_visualisation(
        double open,
        double height,
        double pin_r,
        double pin_h
) const {
    Vec3 veps = { 0, 0, 0.001 };
    auto upper_lever_wheel = _pinned_wheel_with_arc(
            upper_lever_length(), height,
            pin_r, pin_h,
            { 0, 1, 0, 0.4 }
    );
    upper_lever_wheel.translate(_vec3(_upper_axis) + veps*2);
    auto moving_wheel = _pinned_wheel_with_arc(
            cover_mp_distance(), height,
            pin_r, pin_h,
            { 1, 0, 1, 0.4 }
    );
    double upper_angle = get_upper_lever_angle(open, true);
    Vec3 upper_ax = _vec3(_upper_axis);
    Vec3 upper_mp = upper_ax + Vec3(upper_lever_length(), 0, 0);
    upper_mp = _rotate2D(upper_ax, upper_mp, upper_angle);
    moving_wheel.translate(upper_mp);
    auto lower_lever_wheel = _pinned_wheel_with_arc(
            lower_lever_length(), height,
            pin_r, pin_h,
            { 0, 0, 1, 0.4 }
    );
    lower_lever_wheel.translate(_vec3(_lower_axis) + veps);
    double lower_angle = get_lower_lever_angle(open, true);
    Vec3 lower_ax = _vec3(_lower_axis);
    Vec3 lower_mp = lower_ax + Vec3(lower_lever_length(), 0, 0);
    lower_mp = _rotate2D(lower_ax, lower_mp, lower_angle);
    auto n0 = nail(lower_mp, {1,1,1});
    auto n1 = nail(_vec3(_upper_axis));
    auto n2 = nail(_vec3(_upper_mp0));
    auto n3 = nail(_vec3(_upper_mp1));
    auto n4 = nail(_vec3(_lower_axis));
    auto n5 = nail(_vec3(_lower_mp));
    HelperPart nails = n0 + n1 + n2 + n3 + n4 + n5;
    return nails + lower_lever_wheel + upper_lever_wheel + moving_wheel;
}


Vec3 NonLinearLeverCalculator::_rotate2D(
        Vec3 axis,
        Vec3 point,
        double angle
) {
    return axis + Vec3(
        (point.x - axis.x) * std::cos(angle)
            - (point.y - axis.y) * std::sin(angle),
        (point.x - axis.x) * std::sin(angle)
            + (point.y - axis.y) * std::cos(angle),
        0
    );
}

std::pair<Vec3, Vec3> NonLinearLeverCalculator::_circles_cross(
        Vec3 center1,
        double r1,
        Vec3 center2,
        double r2
) {
    double centers_dist = (center2 - center1).length();
    if (centers_dist > r1 + r2
     || centers_dist < std::abs(r2 - r1))
        throw std::runtime_error("given circles dont intersect");
    double cos_theta = centers_dist*centers_dist + r1*r1 - r2*r2;
    cos_theta /= 2 * centers_dist * r1;
    double sin_theta = std::sqrt(1 - cos_theta*cos_theta);
    Vec3 vp = (center2 - center1) * (r1 / centers_dist);
    Vec3 intersect1 = {
        vp.x*cos_theta - vp.y*sin_theta + center1.x,
        vp.x*sin_theta + vp.y*cos_theta + center1.y,
        0
    };
    Vec3 intersect2 = {
        vp.x*cos_theta + vp.y*sin_theta + center1.x,
        - vp.x*sin_theta + vp.y*cos_theta + center1.y,
        0
    };
    return {intersect1, intersect2};
}


HelperPart NonLinearLeverCalculator::_pinned_wheel_with_arc(
        double r, double h,
        double pin_r, double pin_h,
        Color color
) {
    auto wheel = Cylinder::create(r, r, h);
    wheel.color(color.x, color.y, color.z, color.alpha);
    color.alpha = 1;
    auto center = nail(Vec3::ZERO(), color, pin_r, pin_h);
    center.color(color.x, color.y, color.z, color.alpha);
    return center + wheel;
}

