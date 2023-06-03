#ifndef PACKER_H_INCLUDED
#define PACKER_H_INCLUDED

#include "base/geometry.h"
#include "base/parts.hpp"
#include "core/Component2D.h"
#include "core/IndentWriter.h"
#include <vector>


struct Sheet {
    Sheet(double thickness, Vec2 dims)
        : thickness { thickness }
        , dims { dims }
    { }

    void add(const Component2D& component)
    {
        elements.push_back(component);
    }

    void render(IndentWriter& writer)
    {
        for (auto& element : elements)
            writer << element;
    }

    double thickness;
    Vec2 dims;
    std::vector<Component2D> elements;
}; // struct Sheet


class IPacker {
  public:
    virtual void set_dims(Vec2) = 0;

    virtual size_t add_part(const FlatPart&) = 0;

    virtual size_t add_part(const Module3D&) = 0;

    // virtual size_t add_part(const Part3D&) = 0;

    template <typename... ArgTs>
    size_t add_parts(const ArgTs&... args)
    {
        return (add_part(args) + ...);
    }

    virtual std::vector<Sheet> pack() = 0;
}; // class Packer


#endif // PACKER_H_INCLUDED
