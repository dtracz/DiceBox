#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "components.h"
#include "base.h"
#include "toolkit.h"
#include "globals.h"


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


Module3D get(const IColorGenerator& colors) {
    auto deck = get_deck(colors[0]);
    auto front_x_wall = get_x_wall(colors[1])
            .rotate({-90, 0, 0})
            .translate({0, OW_THICC, 0});
    auto back_x_wall = get_x_wall(colors[1])
            .rotate({-90, 0, 0})
            .translate({0, FULL_DIMS.y, 0});
    auto left_y_wall = get_left_y_wall(colors[2])
            .rotate({0, -90, 0})
            .translate({0, 0, FULL_DIMS.z});
    auto right_y_wall = get_right_y_wall(colors[2])
            .rotate({0, -90, 0})
            .translate({FULL_DIMS.x - OW_THICC, 0, FULL_DIMS.z});
    auto y_separator1 = get_y_separator(colors[2])
            .rotate({-90, 0, -90})
            .translate({FULL_DIMS.x/2 - IW_THICC, 0, 0});
    auto y_separator2 = get_y_separator(colors[3])
            .rotate({-90, 0, -90})
            .translate({FULL_DIMS.x/2, 0, 0});
    auto x_separator = get_x_separator(colors[1])
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

