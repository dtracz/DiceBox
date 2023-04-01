#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "components.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"
#include "upper_part.h"


namespace UpperPart {

const double DZ = (SLOT_SIZE.z-RS)/2;
const double TLHD = 3; // top lever hole diameter;

const double MP_X = RS*3/5;
const double MP_Y = RS*2/5;


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


Module3D get(const IColorGenerator& colors) {
    auto deck = get_deck(colors[0])
            .translate({OW_THICC, 0, 0});
    auto front_x_wall = get_x_wall(colors[1])
            .rotate({-90, 0, 0})
            .translate({OW_THICC, OW_THICC, 0});
    auto back_x_wall = get_x_wall(colors[1])
            .rotate({-90, 0, 0})
            .translate({OW_THICC, FULL_DIMS.y, 0});
    auto y_separator = get_y_separator(colors[2])
            .rotate({0, -90, 0})
            .translate({OW_THICC + SLOT_SIZE.x, 0, FULL_DIMS.z - RS});
    auto x_separator = get_x_separator(colors[1])
            .rotate({-90, 0, 0})
            .translate({OW_THICC, IW_THICC + OW_THICC + SLOT_SIZE.y, 0});
    auto lower_part = Module3D::Union(
            deck,
            front_x_wall,
            back_x_wall,
            y_separator,
            x_separator
    );
    return lower_part;
}

}  // namespace UpperPart

