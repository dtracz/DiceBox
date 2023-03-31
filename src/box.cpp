#include <iostream>
#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "components.h"
#include "base.h"
#include "toolkit.h"


const double OW_THICC = 6;
const double IW_THICC = 4;
const Vec3 SLOT_SIZE {50, 50, 25};
const Vec3 FULL_DIMS {
    OW_THICC*2 + IW_THICC*2 + SLOT_SIZE.x*2,
    OW_THICC*2 + IW_THICC + SLOT_SIZE.y*2,
    OW_THICC + SLOT_SIZE.z
};
const double CZ = (FULL_DIMS.z - OW_THICC) / 5;
const double CX = (FULL_DIMS.x - 2*OW_THICC) / 11;
const double DX = SLOT_SIZE.x / 5;
const double CY = FULL_DIMS.y / 13;
const double SIDE_CUT = 2*CZ;
const double BOTT_CUT = CY;
const double CENT_CUT = CZ;


namespace LowerPart {

FlatPart get_deck(Color color) {
    FlatPart deck {crenellated_wall(
            {FULL_DIMS.x, FULL_DIMS.y},
            {CX, OW_THICC, 0}, {CX, OW_THICC, 0},
            {CY, OW_THICC, CY}, {CY, OW_THICC, CY}
    )};
    deck -= Square::create(2*IW_THICC, BOTT_CUT, false).translate(
            OW_THICC + SLOT_SIZE.x, OW_THICC, 0
    );
    deck -= Square::create(2*IW_THICC, BOTT_CUT, false).translate(
            OW_THICC + SLOT_SIZE.x, FULL_DIMS.y - OW_THICC - BOTT_CUT, 0
    );
    deck -= Square::create(BOTT_CUT, IW_THICC, false).translate(
            OW_THICC, OW_THICC + SLOT_SIZE.y, 0
    );
    deck.set_thickness(OW_THICC);
    deck.set_color(color);
    return deck;
}


FlatPart get_x_wall(Color color) {
    FlatPart wall {crenellated_wall(
            {FULL_DIMS.x, FULL_DIMS.z},
            Vec3::ZERO(), {CX, OW_THICC, CX},
            {CZ, OW_THICC, 0}, {CZ, OW_THICC, 0}
    )};
    wall -= Square::create(OW_THICC, CZ, false);
    wall -= Square::create(OW_THICC, CZ, false).translate(
            FULL_DIMS.x-OW_THICC, 0, 0
    );
    wall -= Square::create(2*IW_THICC, SIDE_CUT, false).translate(
            OW_THICC + SLOT_SIZE.x, OW_THICC + SLOT_SIZE.z - SIDE_CUT, 0
    );
    wall.set_thickness(OW_THICC);
    wall.set_color(color);
    return wall;
}


FlatPart get_right_y_wall(Color color) {
    FlatPart wall {crenellated_wall(
            {FULL_DIMS.z, FULL_DIMS.y},
            {CZ, OW_THICC, CZ}, {CZ, OW_THICC, CZ},
            Vec3::ZERO(), {CY, OW_THICC, 0}
    )};
    wall.set_thickness(OW_THICC);
    wall.set_color(color);
    return wall;
}


FlatPart get_left_y_wall(Color color) {
    auto wall = get_right_y_wall({-1});
    wall -= Square::create(SIDE_CUT, IW_THICC, false).translate(
            0, OW_THICC+SLOT_SIZE.y, 0
    );
    wall.set_color(color);
    return wall;
}


FlatPart get_y_separator(Color color) {
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(OW_THICC, 0)
            << Point2D(OW_THICC, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z)
            << Point2D(OW_THICC + SLOT_SIZE.y, FULL_DIMS.z)
            << Point2D(OW_THICC + SLOT_SIZE.y, FULL_DIMS.z - CENT_CUT)
            << Point2D(OW_THICC + SLOT_SIZE.y + IW_THICC, FULL_DIMS.z - CENT_CUT)
            << Point2D(OW_THICC + SLOT_SIZE.y + IW_THICC, FULL_DIMS.z)
            << Point2D(FULL_DIMS.y, FULL_DIMS.z)
            << Point2D(FULL_DIMS.y, FULL_DIMS.z - SIDE_CUT)
            << Point2D(FULL_DIMS.y - OW_THICC, FULL_DIMS.z - SIDE_CUT)
            << Point2D(FULL_DIMS.y - OW_THICC, 0)
            << Point2D(FULL_DIMS.y - OW_THICC - BOTT_CUT, 0)
            << Point2D(FULL_DIMS.y - OW_THICC - BOTT_CUT, OW_THICC)
            << Point2D(OW_THICC + SLOT_SIZE.y + IW_THICC, OW_THICC)
            << Point2D(OW_THICC + SLOT_SIZE.y + IW_THICC, OW_THICC + CENT_CUT)
            << Point2D(OW_THICC + SLOT_SIZE.y, OW_THICC + CENT_CUT)
            << Point2D(OW_THICC + SLOT_SIZE.y, OW_THICC)
            << Point2D(OW_THICC + BOTT_CUT, OW_THICC)
            << Point2D(OW_THICC + BOTT_CUT, 0);
    FlatPart sep {sep_base};
    sep.set_thickness(IW_THICC);
    sep.set_color(color);
    return sep;
}


FlatPart get_x_separator(Color color) {
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(OW_THICC, 0)
            << Point2D(OW_THICC, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z)
            << Point2D(FULL_DIMS.x/2 + IW_THICC, FULL_DIMS.z)
            << Point2D(FULL_DIMS.x/2 + IW_THICC, FULL_DIMS.z - CENT_CUT)
            << Point2D(FULL_DIMS.x/2 - IW_THICC, FULL_DIMS.z - CENT_CUT)
            << Point2D(FULL_DIMS.x/2 - IW_THICC, OW_THICC + CENT_CUT)
            << Point2D(FULL_DIMS.x/2 + IW_THICC, OW_THICC + CENT_CUT)
            << Point2D(FULL_DIMS.x/2 + IW_THICC, OW_THICC)
            << Point2D(OW_THICC + BOTT_CUT, OW_THICC)
            << Point2D(OW_THICC + BOTT_CUT, 0);
    FlatPart sep {sep_base};
    sep.set_thickness(IW_THICC);
    sep.set_color(color);
    return sep;
}


Module3D get(Color c0, Color c1, Color c2, Color c3) {
    auto deck = get_deck(c0);
    auto front_x_wall = get_x_wall(c1)
            .rotate({-90, 0, 0})
            .translate({0, OW_THICC, 0});
    auto back_x_wall = get_x_wall(c1)
            .rotate({-90, 0, 0})
            .translate({0, FULL_DIMS.y, 0});
    auto left_y_wall = get_left_y_wall(c2)
            .rotate({0, -90, 0})
            .translate({0, 0, FULL_DIMS.z});
    auto right_y_wall = get_right_y_wall(c2)
            .rotate({0, -90, 0})
            .translate({FULL_DIMS.x - OW_THICC, 0, FULL_DIMS.z});
    auto y_separator1 = get_y_separator(c2)
            .rotate({-90, 0, -90})
            .translate({FULL_DIMS.x/2 - IW_THICC, 0, 0});
    auto y_separator2 = get_y_separator(c3)
            .rotate({-90, 0, -90})
            .translate({FULL_DIMS.x/2, 0, 0});
    auto x_separator = get_x_separator(c1)
            .rotate({-90, 0, 0})
            .translate({0, OW_THICC + SLOT_SIZE.y + IW_THICC, 0});

    auto lower_part = Module3D::Union(
            deck,
            front_x_wall,
            back_x_wall,
            left_y_wall,
            right_y_wall,
            y_separator1,
            y_separator2,
            x_separator
    );
    return lower_part;
}

}  // namespace LowerPart


namespace UpperPart {

const double RS = 15;
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


Module3D get(Color c0, Color c1, Color c2, Color c3) {
    auto deck = get_deck(c0)
            .translate({OW_THICC, 0, 0});
    auto front_x_wall = get_x_wall(c1)
            .rotate({-90, 0, 0})
            .translate({OW_THICC, OW_THICC, 0});
    auto back_x_wall = get_x_wall(c1)
            .rotate({-90, 0, 0})
            .translate({OW_THICC, FULL_DIMS.y, 0});
    auto y_separator = get_y_separator(c2)
            .rotate({0, -90, 0})
            .translate({OW_THICC + SLOT_SIZE.x, 0, FULL_DIMS.z - RS});
    auto x_separator = get_x_separator(c1)
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


int main() {
    auto lp = LowerPart::get({0,1,0}, {1,0,0}, {0,0,1}, {0.5,0,1});
    auto up1 = UpperPart::get({0,1,0}, {1,0,0}, {0,0,1}, {0.5,0,1});
    up1.translate({-FULL_DIMS.x/2, 0, FULL_DIMS.z});
    auto up2 = up1;
    up2.mirror({1,0,0}, {FULL_DIMS.x/2, 0, 0});

    IndentWriter writer;
    lp.render3D(writer);
    up1.render3D(writer);
    up2.render3D(writer);
    std::cout << writer << std::flush;
    return 0;
}

