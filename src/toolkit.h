#ifndef TOOLKIT_H_INCLUDED
#define TOOLKIT_H_INCLUDED

#include <stdexcept>
#include <vector>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <numbers>
#include "core/Component2D.h"
#include "base.h"



Component2D crenels(std::pair<double, double> crenel_dims,
                    double length, double offset=0);


Component2D crenellated_wall(std::pair<double, double> dims,
                             Vec3 top_crenel_params = Vec3::ZERO(),
                             Vec3 bottom_crenel_params = Vec3::ZERO(),
                             Vec3 left_crenel_params = Vec3::ZERO(),
                             Vec3 right_crenel_params = Vec3::ZERO());



class DistinguishableColorGenerator : public IColorGenerator {
  public:
    DistinguishableColorGenerator(size_t offset=0): _offset{offset} { }

    Color next() override;

    Color operator[](size_t idx) const override;

    void set_offset(size_t offset) {
        _offset = offset;
    }

  private:
    static double _get_value(size_t);

    size_t _offset;
    const static Color _basic_colors[6];
};  // DistinguishableColorGenerator



class NonLinearLeverCalculator {
  public:
    NonLinearLeverCalculator(
            const std::pair<double, double>& upper_axis,
            const std::pair<double, double>& upper_mp0,
            const std::pair<double, double>& upper_mp1,
            const std::pair<double, double>& lower_axis,
            const std::pair<double, double>& lower_mp
    ):  _upper_axis(upper_axis),
        _upper_mp0(upper_mp0),
        _upper_mp1(upper_mp1),
        _lower_axis(lower_axis),
        _lower_mp(lower_mp) { }

    double upper_lever_length() const {
        return (_vec3(_upper_mp0) - _vec3(_upper_axis)).length();
    }

    Vec3 upper_axis() const {
        return _vec3(_upper_axis);
    }

    double lower_lever_length() const {
        return (_vec3(_lower_mp) - _vec3(_lower_axis)).length();
    }

    Vec3 lower_axis() const {
        return _vec3(_lower_axis);
    }

    double cover_mp_distance() const {
        return (_vec3(_upper_mp0) - _vec3(_lower_mp)).length();
    }

    double get_upper_lever_angle(double open, bool radians=false) const;

    double get_lower_lever_angle(double open, bool radians=false) const;

    double get_cover_angle(double open, bool radians=false) const;

    std::pair<double, double> get_cover_shift(double open) const;

    Module3D get_visualisation(
            double open,
            double height=2,
            double pin_r=1,
            double pin_h=8
    ) const;

  private:
    std::pair<double, double> _upper_axis;
    std::pair<double, double> _upper_mp0;
    std::pair<double, double> _upper_mp1;
    std::pair<double, double> _lower_axis;
    std::pair<double, double> _lower_mp;

    static Vec3 _vec3(const std::pair<double, double>& vec2) {
        return {vec2.first, vec2.second, 0};
    }

    static double _angle2D(Vec3 vec) {
        double angle = std::atan2(vec.y, vec.x);
        return std::fmod(angle + 2*std::numbers::pi, 2*std::numbers::pi);
    }

    static Vec3 _rotate2D(Vec3, Vec3, double);

    static std::pair<Vec3, Vec3> _circles_cross(Vec3, double, Vec3, double);

    static HelperPart _pinned_wheel_with_arc(
            double r, double h,
            double pin_r, double pin_h,
            Color color
    );

};  // NonLinearLeverCalculator


#endif  // TOOLKIT_H_INCLUDED
