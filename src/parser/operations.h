#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include "base/geometry.h"
#include "parser/shapes.h"
#include <memory>
#include <stdexcept>
#include <vector>


namespace parser {

class Operation : public Parseable {
  public:
    virtual void apply(std::shared_ptr<Shape>) = 0;
}; // class Operation


class Translate : public Operation {
  public:
    Translate(Vec2 translation)
        : _translation { translation }
    { }

    Translate(double x, double y, double z)
        : _translation { x, y }
    {
        if (z != 0)
            throw std::runtime_error("Object translated in z dimension");
    }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->translate(_translation);
    }

    void print()
    {
        printf("Translate\n");
    }

  private:
    Vec2 _translation;
}; // class Translate


class Rotate : public Operation {
  public:
    Rotate(double angle)
        : _angle { angle }
    { }

    Rotate(double x, double y, double z)
        : _angle { _normalize(x, y, z) }
    { }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->rotate(_angle);
    }

    void print()
    {
        printf("Rotate\n");
    }

  private:
    static double _normalize(double x, double y, double z);

    double _angle;
}; // class Rotate


class Mirror : public Operation {
  public:
    Mirror(Vec2 plane)
        : _plane { plane }
    { }

    Mirror(double x, double y, double z)
        : _plane { x, y }
    {
        if (z != 0)
            throw std::runtime_error("Object mirrored in z dimension");
    }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->mirror(_plane);
    }

    void print()
    {
        printf("Mirror\n");
    }

  private:
    Vec2 _plane;
}; // class Mirror

} // namespace packer

#endif // OPERATIONS_H_INCLUDED
