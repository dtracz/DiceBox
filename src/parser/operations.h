#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include "base/geometry.h"
#include "parser/shapes.h"
#include <memory>


namespace parser {

class Operation {
  public:
    virtual void apply(std::shared_ptr<Shape>) = 0;
}; // class Operation


class Translate : public Operation {
  public:
    Translate(Vec2 translation)
        : _translation { translation }
    { }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->translate(_translation);
    }

  private:
    Vec2 _translation;
}; // class Translate


class Rotate : public Operation {
  public:
    Rotate(double angle)
        : _angle { angle }
    { }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->rotate(_angle);
    }

  private:
    double _angle;
}; // class Rotate


class Mirror : public Operation {
  public:
    Mirror(Vec2 plane)
        : _plane { plane }
    { }

    void apply(std::shared_ptr<Shape> shape_ptr)
    {
        shape_ptr->mirror(_plane);
    }

  private:
    Vec2 _plane;
}; // class Mirror

} // namespace packer

#endif // OPERATIONS_H_INCLUDED
