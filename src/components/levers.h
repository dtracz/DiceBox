#ifndef LEVERS_H_INCLUDED
#define LEVERS_H_INCLUDED

#include "base/colors.h"
#include "base/parts.hpp"


namespace levers {

Module3D simple(
    double length,
    double thichness,
    double outer_r,
    double pin_r,
    double pin_length,
    const IColorGenerator& colors
);

}; // namespace levers


#endif // LEVERS_H_INCLUDED
