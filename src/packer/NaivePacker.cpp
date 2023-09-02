#include "packer/NaivePacker.h"
#include "base/parts.hpp"
#include "core/IndentWriter.h"
#include "globals.h"
#include "parser/Preparser.h"
#include "parser/scad_2D_parser.hpp"
#include <cmath>
#include <sstream>
#include <vector>


using namespace packer;


size_t NaivePacker::add_part(const FlatPart& part)
{
    double thickness = part.get_thickness();
    if (_element_sets.contains(thickness))
        _element_sets[thickness].push_back(part);
    else
        _element_sets[thickness] = { part };
    return 1;
}

size_t NaivePacker::add_part(const Module3D& part)
{
    size_t added = 0;
    for (auto& subpart : part.get_all_flats())
        added += add_part(subpart);
    return added;
}

std::vector<FlatPart>
fill_sheet(Sheet* sheet, const std::vector<FlatPart>& parts)
{
    std::vector<FlatPart> leftovers;
    double next_row_position = 0;
    Vec2 position = Vec2::ZERO();
    for (auto part : parts) {
        auto hull = rectangular_hull(part, SHEET_IN_BETWEEN_MARGIN / 2);
        double width = hull[0].x - hull[2].x;
        double height = hull[0].y - hull[2].y;
        if (height > sheet->dims.y || width > sheet->dims.x) {
            std::stringstream msg;
            msg << "One of components is to big (" << width << ", " << height
                << ") to be fitted in given sheet (" << sheet->dims.x << ", "
                << sheet->dims.y << ")";
            throw std::runtime_error(msg.str());
        }
        if (position.x + width > sheet->dims.x) {
            position = { 0, position.y + next_row_position };
            next_row_position = 0;
        }
        if (position.y + height > sheet->dims.y) {
            leftovers.push_back(part);
            continue;
        }
        Vec2 move_by = position - hull[2];
        next_row_position = std::max(next_row_position, height);
        part.translate({ move_by.x, move_by.y, 0 });
        sheet->add(part);
        position.x += width;
    }
    if (leftovers.size() == parts.size())
        throw std::runtime_error(
            "No part was added to the sheet (should never happen)"
        );
    return leftovers;
}

std::vector<Sheet> NaivePacker::pack()
{
    std::vector<Sheet> sheets;
    for (auto& pair : _element_sets) {
        double thickness = pair.first;
        std::vector<FlatPart>& parts = pair.second;
        while (!parts.empty()) {
            Sheet sheet { thickness, _sheet_dims };
            parts = fill_sheet(&sheet, parts);
            sheets.push_back(sheet);
        }
    }
    return sheets;
}


std::vector<Vec2> packer::rectangular_hull(const FlatPart& part, double margin)
{
    IndentWriter writer;
    part.render2D(writer);
    parser::Preparser preparser;
    preparser << writer;
    auto parser_ptr = std::make_shared<parser::Scad2DParser>();
    preparser >> *parser_ptr;
    std::shared_ptr<parser::Shape> shape
        = std::dynamic_pointer_cast<parser::Shape>(parser_ptr->get_parsed());
    std::vector<Vec2> hull = shape->get_hull();
    hull[0] += { margin, margin };
    hull[1] += { -margin, margin };
    hull[2] += { -margin, -margin };
    hull[3] += { margin, -margin };
    return hull;
}
