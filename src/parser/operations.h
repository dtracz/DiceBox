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
        : translation { translation }
    { }

    void apply(std::shared_ptr<Shape>);

  private:
    Vec2 translation;
}; // class Translate


class Rotate : public Operation {
  public:
    void apply(std::shared_ptr<Shape>);

  private:
    double angle;
}; // class Rotate


class Mirror : public Operation {
  public:
    void apply(std::shared_ptr<Shape>);

  private:
    Vec2 plane;
}; // class Mirror

} // namespace packer

#endif // OPERATIONS_H_INCLUDED
