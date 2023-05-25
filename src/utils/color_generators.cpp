#include "utils/color_generators.h"
#include <cmath>


Color DistinguishableColorGenerator::next()
{
    _offset++;
    return this->operator[](0);
}


Color DistinguishableColorGenerator::operator[](size_t idx) const
{
    idx += _offset;
    double value = _get_value(idx / 6);
    Color color = _basic_colors[idx % 6];
    color *= value;
    return color;
}


double DistinguishableColorGenerator::_get_value(size_t idx)
{
    if (idx == 0)
        return 1;
    double denom = std::exp2(1 + std::floor(std::log2(idx)));
    double numer = idx - denom / 2;
    numer = 2 * numer + 1;
    return numer / denom;
}


const Color DistinguishableColorGenerator::_basic_colors[] = {
    { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 },
    { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 },
};
