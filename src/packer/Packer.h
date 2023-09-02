#ifndef PACKER_H_INCLUDED
#define PACKER_H_INCLUDED

#include "base/geometry.h"
#include "base/parts.hpp"
#include "core/IndentWriter.h"
#include <vector>


namespace packer {

struct Sheet {
    Sheet(double thickness, Vec2 dims)
        : thickness { thickness }
        , dims { dims }
    { }

    void add(const FlatPart& part)
    {
        elements.push_back(part);
    }

    void render(IndentWriter& writer)
    {
        for (auto& element : elements)
            element.render2D(writer);
    }

    double thickness;
    Vec2 dims;
    std::vector<FlatPart> elements;
}; // struct Sheet


class IPacker {
  public:
    virtual void set_dims(Vec2) = 0;

    /**
     * returns number of parts actually packed
     * 1 if `part` is a simple part,
     * n if `part` is a module composed with n simple parts
     */
    virtual size_t add_part(const FlatPart& part) = 0;

    /**
     * returns number of parts actually packed
     * 1 if `part` is a simple part,
     * n if `part` is a module composed with n simple parts
     */
    virtual size_t add_part(const Module3D& part) = 0;

    // virtual size_t add_part(const Part3D&) = 0;

    // TODO: unify `add_part` functions into one for below to work
    // template <typename... ArgTs>
    // size_t add_parts(const ArgTs&... args)
    // {
    //     return (add_part(args) + ...);
    // }

    virtual std::vector<Sheet> pack() = 0;
}; // class Packer

} // namespace packer

#endif // PACKER_H_INCLUDED
