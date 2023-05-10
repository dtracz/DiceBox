#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "components.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"
#include "upper_part.h"
#include "levers.h"


namespace UpperPart {

constexpr double DZ = (SLOT_SIZE.z-RS)/2;
// const double TLHD = 3; // top lever hole diameter;

constexpr double MP_X = RS*3/5;
constexpr double MP_Y = RS*2/5;
constexpr double BMP_DEV = -3;

// UpperLever Cover Mount Point
constexpr Vec2 UCMP {
    FULL_DIMS.x/2 - RS + MP_X,
    FULL_DIMS.z - MP_Y
};
// UpperLever Cover Mount Point while Open
constexpr const Vec2 UCMP_O {
    OW_THICC + MP_Y,
    FULL_DIMS.z - RS + MP_X
};

constexpr Vec2 point_on_bisection(
        Vec2 pt1,
        Vec2 pt2,
        double value
) {
    Vec2 result = (pt1 + pt2)/2;
    result.x += -value*(result.y - pt1.y)/(result.x - pt1.x);
    result.y += value;
    return result;
}

// UpperLever Base Mount Point
constexpr Vec2 UBMP { point_on_bisection(UCMP_O, UCMP, BMP_DEV) };

// Upper Lever Length
constexpr double ULL = (UCMP - UBMP).length();

// LowerLever Cover Mount Point while Open
constexpr Vec2 LCMP { OW_THICC/2, OW_THICC/2 };
// LowerLever Cover Mount Point while Open
constexpr Vec2 LCMP_O {
    LCMP.x + FULL_DIMS.z,
    LCMP.y + FULL_DIMS.x/2 - FULL_DIMS.z
};

// Lower Lever Length
const double LLL = (sqr(FULL_DIMS.z) + sqr(FULL_DIMS.x/2 - FULL_DIMS.z))
                 / (2*FULL_DIMS.z);
// LowerLever Base Mount Point
constexpr Vec2 LBMP { LCMP.x + LLL, LCMP.y };



FlatPart get_deck(Color color) {
    FlatPart deck {crenellated_wall(
            {SLOT_SIZE.x + IW_THICC, FULL_DIMS.y},
            {DX, OW_THICC, DX}, {DX, OW_THICC, DX},
            Vec3::ZERO(), {CY, IW_THICC, CY}
    )};
    deck -= Square::create(BOTT_CUT, IW_THICC, false).translate(
            0, OW_THICC + SLOT_SIZE.y, 0
    );
    deck -= Square::create(BOTT_CUT, IW_THICC, false).translate(
            SLOT_SIZE.x - BOTT_CUT, OW_THICC + SLOT_SIZE.y, 0
    );
    deck.set_thickness(OW_THICC);
    deck.set_color(color);
    return deck;
}


FlatPart get_x_wall(Color color) {
    auto trapezoid = Polygon2D::create();
    auto& polygon = trapezoid.getRef<Polygon2D>();
    polygon << Point2D(0, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x + IW_THICC, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x + IW_THICC - RS, FULL_DIMS.z)
            << Point2D(RS, FULL_DIMS.z);
    auto crenellated_part = crenellated_wall(
            {FULL_DIMS.z - RS, SLOT_SIZE.x + IW_THICC},
            {DZ, IW_THICC, DZ}, Vec3::ZERO(),
            Vec3::ZERO(), {DX, OW_THICC, IW_THICC}
    );
    crenellated_part.rotate(0, 0, -90);
    crenellated_part.translate(0, FULL_DIMS.z - RS, 0);
    FlatPart wall {trapezoid + crenellated_part};
    wall.set_thickness(OW_THICC);
    wall.set_color(color);
    return wall;
}


FlatPart get_y_separator(Color color) {
    FlatPart sep {crenellated_wall(
            {FULL_DIMS.z - RS, FULL_DIMS.y},
            {DZ, OW_THICC, 0}, {DZ, OW_THICC, 0},
            Vec3::ZERO(), {CY, OW_THICC, 0}
    )};
    sep -= Square::create(DZ, IW_THICC, false).translate(
            0, OW_THICC + SLOT_SIZE.y, 0
    );
    sep.set_thickness(IW_THICC);
    sep.set_color(color);
    return sep;
}


FlatPart get_x_separator(Color color) {
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(SLOT_SIZE.x + IW_THICC, FULL_DIMS.z - RS)
            << Point2D(SLOT_SIZE.x+IW_THICC-RS, FULL_DIMS.z)
            << Point2D(RS, FULL_DIMS.z)
            << Point2D(0, FULL_DIMS.z-RS)
            << Point2D(0, 0)
            << Point2D(BOTT_CUT, 0)
            << Point2D(BOTT_CUT, OW_THICC)
            << Point2D(SLOT_SIZE.x-BOTT_CUT, OW_THICC)
            << Point2D(SLOT_SIZE.x-BOTT_CUT, 0)
            << Point2D(SLOT_SIZE.x, 0)
            << Point2D(SLOT_SIZE.x, OW_THICC+DZ)
            << Point2D(SLOT_SIZE.x+IW_THICC, OW_THICC+DZ);
    FlatPart sep {sep_base};
    sep.set_thickness(IW_THICC);
    sep.set_color(color);
    return sep;
}


Module3D get_base(const IColorGenerator& colors) {
    auto deck = get_deck(colors[0])
            .translate({0, 0, 0});
    auto front_x_wall = get_x_wall(colors[3])
            .rotate({-90, 0, 0})
            .translate({0, OW_THICC, 0});
    auto back_x_wall = get_x_wall(colors[3])
            .rotate({-90, 0, 0})
            .translate({0, FULL_DIMS.y, 0});
    auto y_separator = get_y_separator(colors[2])
            .rotate({0, -90, 0})
            .translate({0 + SLOT_SIZE.x, 0, FULL_DIMS.z - RS});
    auto x_separator = get_x_separator(colors[3])
            .rotate({-90, 0, 0})
            .translate({0, IW_THICC + OW_THICC + SLOT_SIZE.y, 0});
    auto base = Module3D::Union(
            deck,
            front_x_wall,
            back_x_wall,
            y_separator,
            x_separator
    );
    return base;
}


FlatPart get_top_cover(Color color) {
    FlatPart top_cover {crenellated_wall(
            {OW_THICC + RS, FULL_DIMS.y},
            {RS/3, OW_THICC, RS/3}, {RS/3, OW_THICC, RS/3},
            {CY, OW_THICC, CY}, Vec3::ZERO()
    )};
    top_cover += Square::create(
            SLOT_SIZE.x + IW_THICC - 2*RS, FULL_DIMS.y, false
    ).translate(OW_THICC + RS, 0, 0);
    top_cover += crenellated_wall(
            {RS, FULL_DIMS.y},
            {RS/3, OW_THICC, RS/3}, {RS/3, OW_THICC, RS/3},
            Vec3::ZERO(), {CY, IW_THICC, CY}
    ).translate(FULL_DIMS.x/2 - RS, 0, 0);
    top_cover -= Square::create(RS/2, IW_THICC, false).translate(
            OW_THICC, OW_THICC + SLOT_SIZE.y, 0
    );
    top_cover -= Square::create(RS/3, IW_THICC, false).translate(
            OW_THICC + SLOT_SIZE.x - RS/3, OW_THICC + SLOT_SIZE.y, 0
    );
    top_cover.set_thickness(OW_THICC);
    top_cover.set_color(color);
    return top_cover;
}


FlatPart get_back_wall(Color color) {
    auto wall_upper = crenellated_wall(
        {RS + OW_THICC, FULL_DIMS.y},
        {RS/3, OW_THICC, RS/3}, {RS/3, OW_THICC, RS/3},
        Vec3::ZERO(), {CY, OW_THICC, 0}
    );
    FlatPart wall {wall_upper.translate(FULL_DIMS.z - RS, 0, 0)};
    wall -= Square::create(RS/2, IW_THICC, false).translate(
            FULL_DIMS.z - RS, OW_THICC + SLOT_SIZE.y, 0
    );
    wall += Square::create(FULL_DIMS.z - RS, FULL_DIMS.y, false);
    wall.set_thickness(OW_THICC);
    wall.set_color(color);
    return wall;
}


FlatPart get_front_sep(Color color) {
    FlatPart sep {crenellated_wall(
        {OW_THICC + RS, FULL_DIMS.y - 2*OW_THICC},
        Vec3::ZERO(), Vec3::ZERO(),
        {CY, OW_THICC, -OW_THICC}, Vec3::ZERO()
    ).translate(0, OW_THICC, 0)};
    sep += Square::create(RS/3, OW_THICC, false).translate(
            RS/2, 0, 0
    );
    sep += Square::create(RS/3, OW_THICC, false).translate(
            RS/2, FULL_DIMS.y - OW_THICC, 0
    );
    sep -= Square::create(RS/2, IW_THICC, false).translate(
            OW_THICC + RS/2, OW_THICC + SLOT_SIZE.y, 0
    );
    sep.set_thickness(IW_THICC);
    sep.set_color(color);
    return sep;
}


FlatPart get_outer_back_reinforcement(Color color) {
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(0, 0)
            << Point2D(RS, RS)
            << Point2D(0, RS);
    reinf_base.translate(OW_THICC, 0, 0);
    FlatPart reinf {reinf_base};
    reinf += crenels({RS/3, OW_THICC}, RS)
            .translate(OW_THICC, RS, 0);
    reinf += crenels({RS/3, OW_THICC}, RS)
            .rotate(0, 0, -90)
            .translate(0, RS, 0);
    reinf.set_thickness(OW_THICC);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_outer_front_reinforcement(Color color) {
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(RS, 0)
            << Point2D(RS, RS)
            << Point2D(0, RS);
    FlatPart reinf {reinf_base};
    reinf += crenels({RS/3, OW_THICC}, RS)
            .translate(0, RS, 0);
    reinf -= Square::create(IW_THICC, RS/3, false)
            .translate(RS - IW_THICC, RS + OW_THICC - RS/2 -RS/3, 0);
    reinf.set_thickness(OW_THICC);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_central_back_reinforcement(Color color) {
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(0, 0)
            << Point2D(OW_THICC, 0)
            << Point2D(OW_THICC+RS, RS)
            << Point2D(OW_THICC+RS/2, RS)
            << Point2D(OW_THICC+RS/2, OW_THICC+RS)
            << Point2D(RS-IW_THICC-RS/3, RS+OW_THICC)
            << Point2D(RS-IW_THICC-RS/3, RS/2)
            << Point2D(0, RS/2);
    FlatPart reinf {reinf_base};
    reinf.set_thickness(IW_THICC);
    reinf.set_color(color);
    return reinf;
}


FlatPart get_central_front_reinforcement(Color color) {
    auto reinf_base = Polygon2D::create();
    auto& polygon = reinf_base.getRef<Polygon2D>();
    polygon << Point2D(RS, 0)
            << Point2D(RS, RS/2)
            << Point2D(RS-IW_THICC, RS/2)
            << Point2D(RS-IW_THICC, RS+OW_THICC)
            << Point2D(RS-IW_THICC-RS/3, RS+OW_THICC)
            << Point2D(RS-IW_THICC-RS/3, RS)
            << Point2D(0, RS);
    FlatPart reinf {reinf_base};
    reinf.set_thickness(IW_THICC);
    reinf.set_color(color);
    return reinf;
}


Module3D get_cover(const IColorGenerator& colors) {
    auto top_cover = get_top_cover(colors[0])
            .translate({0, 0, FULL_DIMS.z});
    auto back_wall = get_back_wall(colors[1])
            .rotate({0, 90, 0})
            .translate({OW_THICC, 0, 0});
    auto front_sep = get_front_sep(colors[1])
            .rotate({0, -90, 0})
            .translate({OW_THICC + SLOT_SIZE.x, 0, FULL_DIMS.z + OW_THICC});
    auto outer_back_reinf1 = get_outer_back_reinforcement(colors[4])
            .rotate({-90, 0, 0})
            .translate({0, OW_THICC, FULL_DIMS.z - RS});
    auto outer_back_reinf2 = outer_back_reinf1;
    outer_back_reinf2.mirror({0, 1, 0}, {0, FULL_DIMS.y/2, 0});
    auto outer_front_reinf1 = get_outer_front_reinforcement(colors[4])
            .rotate({-90, 0, 0})
            .translate({FULL_DIMS.x/2 - RS, OW_THICC, FULL_DIMS.z - RS});
    auto outer_front_reinf2 = outer_front_reinf1;
    outer_front_reinf2.mirror({0, 1, 0}, {0, FULL_DIMS.y/2, 0});
    auto central_back_reinf = get_central_back_reinforcement(colors[4])
            .rotate({-90, 0, 0})
            .translate({
                    0,
                    IW_THICC + OW_THICC + SLOT_SIZE.y,
                    FULL_DIMS.z - RS
             });
    auto central_front_reinf = get_central_front_reinforcement(colors[4])
            .rotate({-90, 0, 0})
            .translate({
                    FULL_DIMS.x/2 - RS,
                    IW_THICC + OW_THICC + SLOT_SIZE.y,
                    FULL_DIMS.z - RS
             });
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
        const NonLinearLeverCalculator& calc, double open,
        double width, double thicc, double pin_r,
        const IColorGenerator& colors
) {
    double pin_length = OW_THICC;
    auto lever1 = levers::simple(
            calc.upper_lever_length(), thicc, width/2,
            pin_r, pin_length, colors
    );
    lever1.rotate({0, 0, -calc.get_upper_lever_angle(open)});
    lever1.translate(Vec3::fromXY(calc.upper_axis()));
    lever1.rotate({-90, 0, 0});
    auto lever2 = lever1;
    lever2.mirror({0, 1, 0}, FULL_DIMS/2);
    auto lever3 = levers::simple(
            calc.lower_lever_length(), thicc, width/2,
            pin_r, pin_length, colors
    );
    lever3.rotate({0, 0, -calc.get_lower_lever_angle(open)});
    lever3.translate(Vec3::fromXY(calc.lower_axis()));
    lever3.rotate({-90, 0, 0});
    auto lever4 = lever3;
    lever4.mirror({0, 1, 0}, FULL_DIMS/2);
    return lever1 + lever2 + lever3 + lever4;
}


Module3D get(const IColorGenerator& colors, double open, bool vis) {
    NonLinearLeverCalculator calc {
        UBMP, UCMP, UCMP_O,
        LBMP, LCMP
    };
    auto cover_shift = calc.get_cover_shift(open);
    auto cover_angle = calc.get_cover_angle(open);
    auto base = get_base(colors)
            .translate({OW_THICC, 0, 0});
    Module3D cover = get_cover(colors);
    cover.rotate({0, cover_angle, 0}, Vec3::fromXZ(LCMP));
    cover.translate(Vec3::fromXZ(cover_shift));
    auto levers = get_levers(calc, open, 5, IW_THICC, 1, colors);
    auto upper_part = Module3D::Union(
            base,
            cover,
            levers
    );
    if (vis == false)
        return upper_part;
    auto visualisation = calc.get_visualisation(open)
            .rotate({-90, 0, 0})
            .translate({0, -3, 0});
    return upper_part + visualisation;
}

}  // namespace UpperPart

