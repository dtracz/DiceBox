#ifndef UPPER_PART_H_INCLUDED
#define UPPER_PART_H_INCLUDED

#include "base/parts.hpp"
#include "base/colors.h"


namespace UpperPart {

constexpr double RS = 15;

Module3D get(const IColorGenerator&, double open, bool vis=false);

}  // namespace UpperPart


#endif  // UPPER_PART_H_INCLUDED
