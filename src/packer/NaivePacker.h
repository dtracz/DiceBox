#ifndef NAIVE_PACKER_H_INCLUDED
#define NAIVE_PACKER_H_INCLUDED

#include "base/geometry.h"
#include "base/parts.hpp"
#include "packer/Packer.h"
#include <map>
#include <vector>


namespace packer {

class NaivePacker : public IPacker {
  public:
    NaivePacker(Vec2 sheet_dims)
        : _sheet_dims { sheet_dims }
    { }

    void set_dims(Vec2 sheet_dims) override
    {
        _sheet_dims = sheet_dims;
    }

    size_t add_part(const FlatPart& part) override;

    size_t add_part(const Module3D& part) override;

    std::vector<Sheet> pack() override;

  private:
    Vec2 _sheet_dims;
    std::map<double, std::vector<FlatPart>> _element_sets;
}; // NaivePacker


/**
 * Fit given `FlatPart` into rectangle with given `margin` on each side.
 * Return coordinates of rectangle's corners in order:
 * top-right, top-left, bottom-left, bottom-right
 */
std::vector<Vec2> rectangular_hull(const FlatPart& part, double margin);

} // namespace packer

#endif // NAIVE_PACKER_H_INCLUDED
