#ifndef TOOLKIT_H_INCLUDED
#define TOOLKIT_H_INCLUDED

#include <stdexcept>
#include <vector>
#include <utility>
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
};

#endif  // TOOLKIT_H_INCLUDED
