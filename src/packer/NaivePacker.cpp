#include "packer/NaivePacker.h"
#include "base/parts.hpp"
#include "core/Component2D.h"
#include "core/IndentWriter.h"
#include "globals.h"
#include <vector>


class ShapeThief : public IndentWriter {
  public:
    void operator<<(const Component2D& shape)
    {
        _shape = shape;
    }

    Component2D get()
    {
        return _shape;
    }

  private:
    Component2D _shape;
}; // class ShapeThief


size_t NaivePacker::add_part(const FlatPart& part)
{
    ShapeThief thief;
    part.render2D(thief);
    _add_component(part.get_thickness(), thief.get());
    return 1;
}

size_t NaivePacker::add_part(const Module3D& module3D)
{
    size_t added = 0;
    for (auto& part : module3D.get_all_flats())
        added += add_part(part);
    return added;
}

std::vector<Component2D>
fill_sheet(Sheet* sheet, const std::vector<Component2D>& components)
{
    std::vector<Component2D> leftovers;
    double next_row_position = 0;
    Vec2 position = Vec2::ZERO();
    for (auto component : components) {
        auto hull = rectangular_hull(component, SHEET_IN_BETWEEN_MARGIN / 2);
        double width = hull[0].x - hull[2].x;
        double height = hull[2].y - hull[0].y;
        if (height > sheet->dims.y || width > sheet->dims.x)
            throw std::runtime_error(
                "One of components is to big to be fitted in given sheet"
            );
        if (position.x + width > sheet->dims.x) {
            position = { 0, next_row_position };
            next_row_position = 0;
        }
        if (position.y + height > sheet->dims.y) {
            leftovers.push_back(component);
            continue;
        }
        Vec2 move_by = hull[2] - position;
        component.translate(move_by.x, move_by.y, 0);
        sheet->add(component);
        position.x += width;
    }
    if (leftovers.size() == components.size())
        throw std::runtime_error(
            "No component was added to the sheet (should never happen)"
        );
    return leftovers;
}

std::vector<Sheet> NaivePacker::pack()
{
    std::vector<Sheet> sheets;
    for (auto& pair : _element_sets) {
        double thickness = pair.first;
        std::vector<Component2D> components = pair.second;
        while (!components.empty()) {
            Sheet sheet { thickness, _sheet_dims };
            components = fill_sheet(&sheet, components);
            sheets.push_back(sheet);
        }
    }
    return sheets;
}

void NaivePacker::_add_component(double thickness, const Component2D& component)
{
    if (_element_sets.contains(thickness))
        _element_sets[thickness].push_back(component);
    else
        _element_sets[thickness] = { component };
}

std::vector<Vec2> rectangular_hull(const Component2D& component, double margin)
{
    // TODO
}
