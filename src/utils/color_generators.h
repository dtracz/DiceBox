#ifndef COLOR_GENERATORS_H_INCLUDED
#define COLOR_GENERATORS_H_INCLUDED

#include "base/colors.h"


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


#endif  // COLOR_GENERATORS_H_INCLUDED
