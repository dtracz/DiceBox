#include "components/levers.h"
#include "base/parts.hpp"
#include "components/Circle.h"
#include "components/Cylinder.h"
#include "components/Square.h"
#include "core/Difference.h"
#include "globals.h"


namespace levers {

HelperPart get_bearing(double r1, double r2, double h)
{
    auto outer = Cylinder::create(r2, h);
    auto inner = Cylinder::create(r1, h + 1);
    HelperPart bearing { (outer - inner).translate(0, 0, h / 2) };
    bearing.set_color({ 0, 0, 0 });
    return bearing;
}


HelperPart get_simple_pin(double r, double h)
{
    return Cylinder::create(r, h).translate(0, 0, h / 2);
}


Module3D simple(
    double length,
    double thickness,
    double outer_r,
    double pin_r,
    double pin_length,
    const IColorGenerator& colors
)
{
    double bearing_r = (pin_r + outer_r) / 2;
    FlatPart lever {
        Square::create(length, 2 * outer_r, false).translate(0, -outer_r, 0)
    };
    lever += Circle::create(outer_r);
    lever += Circle::create(outer_r).translate(length, 0, 0);
    lever -= Circle::create(bearing_r);
    lever -= Circle::create(bearing_r).translate(length, 0, 0);
    lever.set_thickness(thickness);
    lever.set_color(colors[0]);
    auto bearing1 = get_bearing(pin_r, bearing_r, thickness);
    auto bearing2 = get_bearing(pin_r, bearing_r, thickness)
                        .translate({ length, 0, 0 });
    auto pin1 = get_simple_pin(pin_r, thickness + pin_length)
                    .translate({ 0, 0, -pin_length });
    pin1.set_color(colors[1]);
    auto pin2 = get_simple_pin(pin_r, thickness + pin_length)
                    .translate({ length, 0, -pin_length });
    pin2.set_color(colors[1]);
    return lever + bearing1 + pin1 + bearing2 + pin2;
}

}; // namespace levers
