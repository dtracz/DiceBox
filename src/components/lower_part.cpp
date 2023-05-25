#include "base/colors.h"
#include "base/geometry.h"
#include "base/parts.hpp"
#include "components.h"
#include "core/Component2D.h"
#include "core/Polygon2D.h"
#include "globals.h"
#include "utils/structures.h"


namespace LowerPart {

FlatPart get_deck(Color color)
{
    FlatPart deck { crenellated_wall(
        { FULL_DIMS.x, FULL_DIMS.y },
        { CX, OW_THICKNESS, 0 },
        { CX, OW_THICKNESS, 0 },
        { CY, OW_THICKNESS, CY },
        { CY, OW_THICKNESS, CY }
    ) };
    deck -= Square::create(2 * IW_THICKNESS, BOTT_CUT, false)
                .translate(OW_THICKNESS + SLOT_SIZE.x, OW_THICKNESS, 0);
    deck -= Square::create(2 * IW_THICKNESS, BOTT_CUT, false)
                .translate(
                    OW_THICKNESS + SLOT_SIZE.x,
                    FULL_DIMS.y - OW_THICKNESS - BOTT_CUT,
                    0
                );
    deck -= Square::create(BOTT_CUT, IW_THICKNESS, false)
                .translate(OW_THICKNESS, OW_THICKNESS + SLOT_SIZE.y, 0);
    deck.set_thickness(OW_THICKNESS);
    deck.set_color(color);
    return deck;
}


FlatPart get_x_wall(Color color)
{
    FlatPart wall { crenellated_wall(
        { FULL_DIMS.x, FULL_DIMS.z },
        Vec3::ZERO(),
        { CX, OW_THICKNESS, CX },
        { CZ, OW_THICKNESS, 0 },
        { CZ, OW_THICKNESS, 0 }
    ) };
    wall -= Square::create(OW_THICKNESS, CZ, false);
    wall -= Square::create(OW_THICKNESS, CZ, false)
                .translate(FULL_DIMS.x - OW_THICKNESS, 0, 0);
    wall -= Square::create(2 * IW_THICKNESS, SIDE_CUT, false)
                .translate(
                    OW_THICKNESS + SLOT_SIZE.x,
                    OW_THICKNESS + SLOT_SIZE.z - SIDE_CUT,
                    0
                );
    wall.set_thickness(OW_THICKNESS);
    wall.set_color(color);
    return wall;
}


FlatPart get_right_y_wall(Color color)
{
    FlatPart wall { crenellated_wall(
        { FULL_DIMS.z, FULL_DIMS.y },
        { CZ, OW_THICKNESS, CZ },
        { CZ, OW_THICKNESS, CZ },
        Vec3::ZERO(),
        { CY, OW_THICKNESS, 0 }
    ) };
    wall.set_thickness(OW_THICKNESS);
    wall.set_color(color);
    return wall;
}


FlatPart get_left_y_wall(Color color)
{
    auto wall = get_right_y_wall({ -1 });
    wall -= Square::create(SIDE_CUT, IW_THICKNESS, false)
                .translate(0, OW_THICKNESS + SLOT_SIZE.y, 0);
    wall.set_color(color);
    return wall;
}


FlatPart get_y_separator(Color color)
{
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(OW_THICKNESS, 0)
            << Point2D(OW_THICKNESS, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z - SIDE_CUT) << Point2D(0, FULL_DIMS.z)
            << Point2D(OW_THICKNESS + SLOT_SIZE.y, FULL_DIMS.z)
            << Point2D(OW_THICKNESS + SLOT_SIZE.y, FULL_DIMS.z - CENT_CUT)
            << Point2D(
                   OW_THICKNESS + SLOT_SIZE.y + IW_THICKNESS,
                   FULL_DIMS.z - CENT_CUT
               )
            << Point2D(OW_THICKNESS + SLOT_SIZE.y + IW_THICKNESS, FULL_DIMS.z)
            << Point2D(FULL_DIMS.y, FULL_DIMS.z)
            << Point2D(FULL_DIMS.y, FULL_DIMS.z - SIDE_CUT)
            << Point2D(FULL_DIMS.y - OW_THICKNESS, FULL_DIMS.z - SIDE_CUT)
            << Point2D(FULL_DIMS.y - OW_THICKNESS, 0)
            << Point2D(FULL_DIMS.y - OW_THICKNESS - BOTT_CUT, 0)
            << Point2D(FULL_DIMS.y - OW_THICKNESS - BOTT_CUT, OW_THICKNESS)
            << Point2D(OW_THICKNESS + SLOT_SIZE.y + IW_THICKNESS, OW_THICKNESS)
            << Point2D(
                   OW_THICKNESS + SLOT_SIZE.y + IW_THICKNESS,
                   OW_THICKNESS + CENT_CUT
               )
            << Point2D(OW_THICKNESS + SLOT_SIZE.y, OW_THICKNESS + CENT_CUT)
            << Point2D(OW_THICKNESS + SLOT_SIZE.y, OW_THICKNESS)
            << Point2D(OW_THICKNESS + BOTT_CUT, OW_THICKNESS)
            << Point2D(OW_THICKNESS + BOTT_CUT, 0);
    FlatPart sep { sep_base };
    sep.set_thickness(IW_THICKNESS);
    sep.set_color(color);
    return sep;
}


FlatPart get_x_separator(Color color)
{
    auto sep_base = Polygon2D::create();
    auto& polygon = sep_base.getRef<Polygon2D>();
    polygon << Point2D(OW_THICKNESS, 0)
            << Point2D(OW_THICKNESS, FULL_DIMS.z - SIDE_CUT)
            << Point2D(0, FULL_DIMS.z - SIDE_CUT) << Point2D(0, FULL_DIMS.z)
            << Point2D(FULL_DIMS.x / 2 + IW_THICKNESS, FULL_DIMS.z)
            << Point2D(FULL_DIMS.x / 2 + IW_THICKNESS, FULL_DIMS.z - CENT_CUT)
            << Point2D(FULL_DIMS.x / 2 - IW_THICKNESS, FULL_DIMS.z - CENT_CUT)
            << Point2D(FULL_DIMS.x / 2 - IW_THICKNESS, OW_THICKNESS + CENT_CUT)
            << Point2D(FULL_DIMS.x / 2 + IW_THICKNESS, OW_THICKNESS + CENT_CUT)
            << Point2D(FULL_DIMS.x / 2 + IW_THICKNESS, OW_THICKNESS)
            << Point2D(OW_THICKNESS + BOTT_CUT, OW_THICKNESS)
            << Point2D(OW_THICKNESS + BOTT_CUT, 0);
    FlatPart sep { sep_base };
    sep.set_thickness(IW_THICKNESS);
    sep.set_color(color);
    return sep;
}


Module3D get(const IColorGenerator& colors)
{
    auto deck = get_deck(colors[0]);
    auto front_x_wall = get_x_wall(colors[1])
                            .rotate({ -90, 0, 0 })
                            .translate({ 0, OW_THICKNESS, 0 });
    auto back_x_wall = get_x_wall(colors[1])
                           .rotate({ -90, 0, 0 })
                           .translate({ 0, FULL_DIMS.y, 0 });
    auto left_y_wall = get_left_y_wall(colors[2])
                           .rotate({ 0, -90, 0 })
                           .translate({ 0, 0, FULL_DIMS.z });
    auto right_y_wall = get_right_y_wall(colors[2])
                            .rotate({ 0, -90, 0 })
                            .translate(
                                { FULL_DIMS.x - OW_THICKNESS, 0, FULL_DIMS.z }
                            );
    auto y_separator1 = get_y_separator(colors[2])
                            .rotate({ -90, 0, -90 })
                            .translate({ FULL_DIMS.x / 2 - IW_THICKNESS, 0, 0 }
                            );
    auto y_separator2 = get_y_separator(colors[3])
                            .rotate({ -90, 0, -90 })
                            .translate({ FULL_DIMS.x / 2, 0, 0 });
    auto x_separator
        = get_x_separator(colors[1])
              .rotate({ -90, 0, 0 })
              .translate({ 0, OW_THICKNESS + SLOT_SIZE.y + IW_THICKNESS, 0 });

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

} // namespace LowerPart
