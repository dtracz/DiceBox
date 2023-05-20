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



inline constexpr double sqr(double x) {
    return x*x;
}


inline double angle2D(double x, double y) {
    double angle = std::atan2(y, x);
    return std::fmod(angle + 2*std::numbers::pi, 2*std::numbers::pi);
}

inline double angle2D(Vec2 vec) {
    double angle = std::atan2(vec.y, vec.x);
    return std::fmod(angle + 2*std::numbers::pi, 2*std::numbers::pi);
}


Vec2 rotate2D(Vec2, Vec2, double);


Component2D crenels(Vec2 crenel_dims,
                    double length, double offset=0);


Component2D crenellated_wall(Vec2 dims,
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



class LinearLeverCalculator {
  public:
    LinearLeverCalculator(
            Vec2 lower_mp, Vec2 upper_mp0, double shift_to_2nd
    ):  _lower_mp { lower_mp },
        _upper_mp0 { upper_mp0 },
        _shift_to_2nd { shift_to_2nd },
        _angle0 { angle2D(upper_mp0 - lower_mp) },
        _range { std::numbers::pi - 2*_angle0 }
    { }

    double lever_length() const {
        return (_upper_mp0 - _lower_mp).length();
    }

    double get_lever_angle(double open, bool radians=false) const {
        double angle = _angle0 + open*_range;
        return radians ? angle : angle * 360 / (2*std::numbers::pi);
    }

    Vec2 lever1_position() const {
        return _lower_mp;
    }

    Vec2 lever2_position() const {
        return lever1_position() + Vec2(_shift_to_2nd, 0);
    }

    Vec2 cover_position(double open) const;

  private:
    Vec2 _lower_mp;
    Vec2 _upper_mp0;
    double _shift_to_2nd;
    double _angle0;
    double _range;


};  // LinearLeverCalculator


class NonLinearLeverCalculator {
  public:
    NonLinearLeverCalculator(
            Vec2 upper_axis, Vec2 upper_mp0, Vec2 upper_mp1,
            Vec2 lower_axis, Vec2 lower_mp
    ):  _upper_axis(upper_axis),
        _upper_mp0(upper_mp0),
        _upper_mp1(upper_mp1),
        _lower_axis(lower_axis),
        _lower_mp(lower_mp) { }

    double upper_lever_length() const {
        return (_upper_mp0 - _upper_axis).length();
    }

    Vec2 upper_axis() const {
        return _upper_axis;
    }

    double lower_lever_length() const {
        return (_lower_mp - _lower_axis).length();
    }

    Vec2 lower_axis() const {
        return _lower_axis;
    }

    double cover_mp_distance() const {
        return (_upper_mp0 - _lower_mp).length();
    }

    double get_upper_lever_angle(double open, bool radians=false) const;

    double get_lower_lever_angle(double open, bool radians=false) const;

    double get_cover_angle(double open, bool radians=false) const;

    Vec2 get_cover_shift(double open) const;

    Module3D get_visualisation(
            double open,
            double height=2,
            double pin_r=1,
            double pin_h=8
    ) const;

  private:
    Vec2 _upper_axis;
    Vec2 _upper_mp0;
    Vec2 _upper_mp1;
    Vec2 _lower_axis;
    Vec2 _lower_mp;


    static std::pair<Vec2, Vec2> _circles_cross(Vec2, double, Vec2, double);

    static HelperPart _pinned_wheel_with_arc(
            double r, double h,
            double pin_r, double pin_h,
            Color color
    );

};  // NonLinearLeverCalculator


#endif  // TOOLKIT_H_INCLUDED
