#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED

#include <cmath>
#include <algorithm>
#include "base/geometry.h"


struct Color : Vec3 {
    double alpha;

    Color(double r=1, double g=1, double b=1, double alpha=1):
        Vec3 {r, g, b}, alpha {alpha} { }

    bool is_valid() {
        return std::min({x,y,z,alpha}) >= 0
            && std::max({x,y,z,alpha}) <= 1;
    }
};


class IColorGenerator {
  public:
    virtual Color next() = 0;
    virtual Color operator[](size_t idx) const = 0;
};

#endif  // COLORS_H_INCLUDED
