#ifndef LEVBERS_H_INCLUDED
#define LEVBERS_H_INCLUDED

#include "base.h"


namespace levers {

Module3D simple(
        double length, double thichness, double outer_r,
        double pin_r, double pin_length,
        const IColorGenerator& colors
);

};  // namespace levers


#endif  // LEVBERS_H_INCLUDED
