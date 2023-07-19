#include "operations.h"
#include "core/Math.h"
#include <stdexcept>

using namespace parser;

double Rotate::_normalize(double x, double y, double z)
{
    if (x != 0 || y != 0) {
        x = mod(x, 360) - 180;
        y = mod(y, 360) - 180;
        z = mod(z - 180, 360);
    }
    if (x != 0 || y != 0)
        throw std::runtime_error("Object rotated out of xy plane");
    return deg2rad(z);
}
