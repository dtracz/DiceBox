#include "upper_part.h"
#include "base/colors.h"
#include "base/geometry.h"
#include "base/parts.hpp"
#include "components.h"
#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "globals.h"
#include "levers.h"
#include "utils/color_generators.h"
#include "utils/lever_calculators.h"
#include "utils/structures.h"


namespace UpperPart {

constexpr double DZ = (SLOT_SIZE.z - RS) / 2;
// const double TLHD = 3; // top lever hole diameter;

constexpr double MP_X = RS * 3 / 5;
constexpr double MP_Y = RS * 2 / 5;
constexpr double BMP_DEV = -3;

// UpperLever Cover Mount Point
constexpr Vec2 UCMP { FULL_DIMS.x / 2 - RS + MP_X, FULL_DIMS.z - MP_Y };
// UpperLever Cover Mount Point while Open
constexpr const Vec2 UCMP_O { OW_THICKNESS + MP_Y, FULL_DIMS.z - RS + MP_X };

constexpr Vec2 point_on_bisection(Vec2 pt1, Vec2 pt2, double value)
{
    Vec2 result = (pt1 + pt2) / 2;
    result.x += -value * (result.y - pt1.y) / (result.x - pt1.x);
    result.y += value;
    return result;
}

// UpperLever Base Mount Point
constexpr Vec2 UBMP { point_on_bisection(UCMP_O, UCMP, BMP_DEV) };

// Upper Lever Length
constexpr double ULL = (UCMP - UBMP).length();

// LowerLever Cover Mount Point while Open
constexpr Vec2 LCMP { OW_THICKNESS / 2, OW_THICKNESS / 2 };
// LowerLever Cover Mount Point while Open
constexpr Vec2 LCMP_O { LCMP.x + FULL_DIMS.z,
                        LCMP.y + FULL_DIMS.x / 2 - FULL_DIMS.z };

// Lower Lever Length
const double LLL = (sqr(FULL_DIMS.z) + sqr(FULL_DIMS.x / 2 - FULL_DIMS.z))
                 / (2 * FULL_DIMS.z);
// LowerLever Base Mount Point
constexpr Vec2 LBMP { LCMP.x + LLL, LCMP.y };



FlatPart get_deck(Color color)
{
    FlatPart deck { crenellated_wall(
        { SLOT_SIZE.x + IW_THICKNESS, FULL_DIMS.y },
        { DX, OW_THICKNESS, DX },
        { DX, OW_THICKNESS, DX },
        Vec3::ZERO(),
        { CY, IW_THICKNESS, CY }
    ) };
    deck -= Square::create(BOTT_CUT, IW_THICKNESS, false)
                .translate(0, OW_THICKNESS + SLOT_SIZE.y, 0);
    deck -= Square::create(BOTT_CUT, IW_THICKNESS, false)
                .translate(
                    SLOT_SIZE.x - BOTT_CUT, OW_THICKNESS + SLOT_SIZE.y, 0
                );
    deck.set_thickness(OW_THICKNESS);
    deck.set_color(color);
    return deck;
}


FlatPart get_x_wall(Color color)
{
    auto trapezoid = Polygon2D::create();
    auto& polygon = trapezoid.getRef<Polygon2D>();
    polygon << Point2D(0, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x + IW_THICKNESS, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x + IW_THICKNESS - RS, FULL_DIMS.z)
            << Point2D(RS, FULL_DIMS.z);
    auto crenellated_part = crenellated_wall(
        { FULL_DIMS.z - RS, SLOT_SIZE.x + IW_THICKNESS },
        { DZ, IW_THICKNESS, DZ },
        Vec3::ZERO(),
        Vec3::ZERO(),
        { DX, OW_THICKNESS, IW_THICKNESS }
    );
    crenellated_part.rotate(0, 0, -90);
    crenellated_part.translate(0, FULL_DIMS.z - RS, 0);
    FlatPart wall { trapezoid + crenellated_part };
    wall.set_thickness(OW_THICKNESS);
    wall.set_color(color);
    return wall;
}


FlatPart get_y_separator(Color color)
{
    FlatPart sep { crenellated_wall(
        { FULL_DIMS.z - RS, FULL_DIMS.y },
        { DZ, OW_THICKNESS, 0 },
        { DZ, OW_THICKNESS, 0 },
        Vec3::ZERO(),
        { CY, OW_THICKNESS, 0 }
    ) };
    sep -= Square::create(DZ, IW_THICKNESS, false)
               .translate(0, OW_THICKNESS + SLOT_SIZE.y, 0);
    sep.set_thickness(IW_THICKNESS);
    sep.set_color(color);
    return sep;
}


FlatPart get_x_separator(Color color)
{
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(SLOT_SIZE.x + IW_THICKNESS, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x + IW_THICKNESS - RS, FULL_DIMS.z)
            << Point2D(RS, FULL_DIMS.z) << Point2D(0, FULL_DIMS.z - RS)
            << Point2D(0, 0) << Point2D(BOTT_CUT, 0)
            << Point2D(BOTT_CUT, OW_THICKNESS)
            << Point2D(SLOT_SIZE.x - BOTT_CUT, OW_THICKNESS)
            << Point2D(SLOT_SIZE.x - BOTT_CUT, 0) << Point2D(SLOT_SIZE.x, 0)
            << Point2D(SLOT_SIZE.x, OW_THICKNESS + DZ)
            << Point2D(SLOT_SIZE.x + IW_THICKNESS, OW_THICKNESS + DZ);
    FlatPart sep { sep_base };
    sep.set_thickness(IW_THICKNESS);
    sep.set_color(color);
    return sep;
}


Module3D get_base(const IColorGenerator& colors)
{
    auto deck = get_deck(colors[0]).translate({ 0, 0, 0 });
    auto front_x_wall = get_x_wall(colors[3])
                            .rotate({ -90, 0, 0 })
                            .translate({ 0, OW_THICKNESS, 0 });
    auto back_x_wall = get_x_wall(colors[3])
                           .rotate({ -90, 0, 0 })
                           .translate({ 0, FULL_DIMS.y, 0 });
    auto y_separator = get_y_separator(colors[2])
                           .rotate({ 0, -90, 0 })
                           .translate({ 0 + SLOT_SIZE.x, 0, FULL_DIMS.z - RS });
    auto x_separator
        = get_x_separator(colors[3])
              .rotate({ -90, 0, 0 })
              .translate({ 0, IW_THICKNESS + OW_THICKNESS + SLOT_SIZE.y, 0 });
    auto base = Module3D::Union(
        deck, front_x_wall, back_x_wall, y_separator, x_separator
    );
    return base;
}


FlatPart get_top_cover(Color color)
{
    FlatPart top_cover { crenellated_wall(
        { OW_THICKNESS + RS, FULL_DIMS.y },
        { RS / 3, OW_THICKNESS, RS / 3 },
        { RS / 3, OW_THICKNESS, RS / 3 },
        { CY, OW_THICKNESS, CY },
        Vec3::ZERO()
    ) };
    top_cover += Square::create(
                     SLOT_SIZE.x + IW_THICKNESS - 2 * RS, FULL_DIMS.y, false
    )
                     .translate(OW_THICKNESS + RS, 0, 0);
    top_cover += crenellated_wall(
                     { RS, FULL_DIMS.y },
                     { RS / 3, OW_THICKNESS, RS / 3 },
                     { RS / 3, OW_THICKNESS, RS / 3 },
                     Vec3::ZERO(),
                     { CY, IW_THICKNESS, CY }
    )
                     .translate(FULL_DIMS.x / 2 - RS, 0, 0);
    top_cover -= Square::create(RS / 2, IW_THICKNESS, false)
                     .translate(OW_THICKNESS, OW_THICKNESS + SLOT_SIZE.y, 0);
    top_cover -= Square::create(RS / 3, IW_THICKNESS, false)
                     .translate(
                         OW_THICKNESS + SLOT_SIZE.x - RS / 3,
                         OW_THICKNESS + SLOT_SIZE.y,
                         0
                     );
    top_cover.set_thickness(OW_THICKNESS);
    top_cover.set_color(color);
    return top_cover;
}


FlatPart get_back_wall(Color color)
{
    auto wall_upper = crenellated_wall(
        { RS + OW_THICKNESS, FULL_DIMS.y },
        { RS / 3, OW_THICKNESS, RS / 3 },
        { RS / 3, OW_THICKNESS, RS / 3 },
        Vec3::ZERO(),
        { CY, OW_THICKNESS, 0 }
    );
    FlatPart wall { wall_upper.translate(FULL_DIMS.z - RS, 0, 0) };
    wall -= Square::create(RS / 2, IW_THICKNESS, false)
                .translate(FULL_DIMS.z - RS, OW_THICKNESS + SLOT_SIZE.y, 0);
    wall += Square::create(FULL_DIMS.z - RS, FULL_DIMS.y, false);
    wall.set_thickness(OW_THICKNESS);
    wall.set_color(color);
    return wall;
}


FlatPart get_front_sep(Color color)
{
    FlatPart sep { crenellated_wall(
                       { OW_THICKNESS + RS, FULL_DIMS.y - 2 * OW_THICKNESS },
                       Vec3::ZERO(),
                       Vec3::ZERO(),
                       { CY, OW_THICKNESS, -OW_THICKNESS },
                       Vec3::ZERO()
    )
                       .translate(0, OW_THICKNESS, 0) };
    sep += Square::create(RS / 3, OW_THICKNESS, false).translate(RS / 2, 0, 0);
    sep += Square::create(RS / 3, OW_THICKNESS, false)
               .translate(RS / 2, FULL_DIMS.y - OW_THICKNESS, 0);
    sep -= Square::create(RS / 2, IW_THICKNESS, false)
               .translate(OW_THICKNESS + RS / 2, OW_THICKNESS + SLOT_SIZE.y, 0);
    sep.set_thickness(IW_THICKNESS);
    sep.set_color(color);
    return sep;
}


FlatPart get_outer_back_reinforcement(Color color)
{
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(0, 0) << Point2D(RS, RS) << Point2D(0, RS);
    reinf_base.translate(OW_THICKNESS, 0, 0);
    FlatPart reinf { reinf_base };
    reinf += crenels({ RS / 3, OW_THICKNESS }, RS)
                 .translate(OW_THICKNESS, RS, 0);
    reinf += crenels({ RS / 3, OW_THICKNESS }, RS)
                 .rotate(0, 0, -90)
                 .translate(0, RS, 0);
    reinf.set_thickness(OW_THICKNESS);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_outer_front_reinforcement(Color color)
{
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(RS, 0) << Point2D(RS, RS) << Point2D(0, RS);
    FlatPart reinf { reinf_base };
    reinf += crenels({ RS / 3, OW_THICKNESS }, RS).translate(0, RS, 0);
    reinf -= Square::create(IW_THICKNESS, RS / 3, false)
                 .translate(
                     RS - IW_THICKNESS, RS + OW_THICKNESS - RS / 2 - RS / 3, 0
                 );
    reinf.set_thickness(OW_THICKNESS);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_central_back_reinforcement(Color color)
{
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(0, 0) << Point2D(OW_THICKNESS, 0)
            << Point2D(OW_THICKNESS + RS, RS)
            << Point2D(OW_THICKNESS + RS / 2, RS)
            << Point2D(OW_THICKNESS + RS / 2, OW_THICKNESS + RS)
            << Point2D(RS - IW_THICKNESS - RS / 3, RS + OW_THICKNESS)
            << Point2D(RS - IW_THICKNESS - RS / 3, RS / 2)
            << Point2D(0, RS / 2);
    FlatPart reinf { reinf_base };
    reinf.set_thickness(IW_THICKNESS);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_central_front_reinforcement(Color color)
{
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(RS, 0) << Point2D(RS, RS / 2)
            << Point2D(RS - IW_THICKNESS, RS / 2)
            << Point2D(RS - IW_THICKNESS, RS + OW_THICKNESS)
            << Point2D(RS - IW_THICKNESS - RS / 3, RS + OW_THICKNESS)
            << Point2D(RS - IW_THICKNESS - RS / 3, RS) << Point2D(0, RS);
    FlatPart reinf { reinf_base };
    reinf.set_thickness(IW_THICKNESS);
    reinf.set_color(color);
    return reinf;
}


Module3D get_cover(const IColorGenerator& colors)
{
    auto top_cover = get_top_cover(colors[0]).translate({ 0, 0, FULL_DIMS.z });
    auto back_wall = get_back_wall(colors[1])
                         .rotate({ 0, 90, 0 })
                         .translate({ OW_THICKNESS, 0, 0 });
    auto front_sep = get_front_sep(colors[1])
                         .rotate({ 0, -90, 0 })
                         .translate({ OW_THICKNESS + SLOT_SIZE.x,
                                      0,
                                      FULL_DIMS.z + OW_THICKNESS });
    auto outer_back_reinf1 = get_outer_back_reinforcement(colors[4])
                                 .rotate({ -90, 0, 0 })
                                 .translate(
                                     { 0, OW_THICKNESS, FULL_DIMS.z - RS }
                                 );
    auto outer_back_reinf2 = outer_back_reinf1;
    outer_back_reinf2.mirror({ 0, 1, 0 }, { 0, FULL_DIMS.y / 2, 0 });
    auto outer_front_reinf1 = get_outer_front_reinforcement(colors[4])
                                  .rotate({ -90, 0, 0 })
                                  .translate({ FULL_DIMS.x / 2 - RS,
                                               OW_THICKNESS,
                                               FULL_DIMS.z - RS });
    auto outer_front_reinf2 = outer_front_reinf1;
    outer_front_reinf2.mirror({ 0, 1, 0 }, { 0, FULL_DIMS.y / 2, 0 });
    auto central_back_reinf = get_central_back_reinforcement(colors[4])
                                  .rotate({ -90, 0, 0 })
                                  .translate({ 0,
                                               IW_THICKNESS + OW_THICKNESS
                                                   + SLOT_SIZE.y,
                                               FULL_DIMS.z - RS });
    auto central_front_reinf = get_central_front_reinforcement(colors[4])
                                   .rotate({ -90, 0, 0 })
                                   .translate({ FULL_DIMS.x / 2 - RS,
                                                IW_THICKNESS + OW_THICKNESS
                                                    + SLOT_SIZE.y,
                                                FULL_DIMS.z - RS });
    auto cover = Module3D::Union(
        top_cover,
        back_wall,
        front_sep,
        outer_back_reinf1,
        outer_back_reinf2,
        outer_front_reinf1,
        outer_front_reinf2,
        central_back_reinf,
        central_front_reinf
    );
    return cover;
}


Module3D get_levers(
    const NonLinearLeverCalculator& calc,
    double open,
    double width,
    double thickness,
    double pin_r,
    const IColorGenerator& colors
)
{
    double pin_length = OW_THICKNESS;
    auto lever1 = levers::simple(
        calc.upper_lever_length(),
        thickness,
        width / 2,
        pin_r,
        pin_length,
        colors
    );
    lever1.rotate({ 0, 0, -calc.get_upper_lever_angle(open) });
    lever1.translate(Vec3::fromXY(calc.upper_axis()));
    lever1.rotate({ -90, 0, 0 });
    auto lever2 = lever1;
    lever2.mirror({ 0, 1, 0 }, FULL_DIMS / 2);
    auto lever3 = levers::simple(
        calc.lower_lever_length(),
        thickness,
        width / 2,
        pin_r,
        pin_length,
        colors
    );
    lever3.rotate({ 0, 0, -calc.get_lower_lever_angle(open) });
    lever3.translate(Vec3::fromXY(calc.lower_axis()));
    lever3.rotate({ -90, 0, 0 });
    auto lever4 = lever3;
    lever4.mirror({ 0, 1, 0 }, FULL_DIMS / 2);
    return lever1 + lever2 + lever3 + lever4;
}


Module3D get(const IColorGenerator& colors, double open, bool vis)
{
    NonLinearLeverCalculator calc { UBMP, UCMP, UCMP_O, LBMP, LCMP };
    auto cover_shift = calc.get_cover_shift(open);
    auto cover_angle = calc.get_cover_angle(open);
    auto base = get_base(colors).translate({ OW_THICKNESS, 0, 0 });
    Module3D cover = get_cover(colors);
    cover.rotate({ 0, cover_angle, 0 }, Vec3::fromXZ(LCMP));
    cover.translate(Vec3::fromXZ(cover_shift));
    auto levers = get_levers(calc, open, 5, IW_THICKNESS, 1, colors);
    auto upper_part = Module3D::Union(base, cover, levers);
    if (vis == false)
        return upper_part;
    auto visualisation = calc.get_visualisation(open)
                             .rotate({ -90, 0, 0 })
                             .translate({ 0, -3, 0 });
    return upper_part + visualisation;
}

} // namespace UpperPart
