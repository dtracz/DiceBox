#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "base/geometry.h"
#include <memory>
#include <vector>


namespace parser {

enum class ShapeTypeId {
    Rectangle,
    Circle,
    Polygon,
    Union,
    Difference,
}; // enum class ShapeTypeId


struct Shape {
    virtual ShapeTypeId type_id() = 0;
}; // struct Shape


struct SimpleShape : public Shape {
    SimpleShape()
        : position { Vec2::ZERO() }
        , rotation { 0 }
    { }

    Vec2 position;
    double rotation;
}; // struct SimpleShape

struct Rectangle : public SimpleShape {
    Rectangle(Vec2 size, bool center)
        : size { size }
        , center { center }
    { }

    Rectangle(double x, double y, bool center)
        : size { x, y }
        , center { center }
    { }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Rectangle;
    }

    Vec2 size;
    bool center;
}; // struct Rectangle

struct Circle : public SimpleShape {
    Circle(double radius)
        : radius { radius }
    { }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Circle;
    }

    double radius;
}; // struct Circle

struct Polygon : public SimpleShape {
    Polygon(const std::vector<Vec2>& vertices)
        : vertices { vertices }
    { }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Polygon;
    }

    std::vector<Vec2> vertices;
}; // struct Polygon


struct ShapeContainer : public Shape {
    std::vector<std::shared_ptr<Shape>> children;
}; // struct ShapeContainer

struct Union : public ShapeContainer {
    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Union;
    }
}; // struct Union

struct Difference : public ShapeContainer {
    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Difference;
    }
}; // struct Difference

} // namespace packer

#endif // SHAPES_H_INCLUDED
