#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "base/geometry.h"
#include <memory>
#include <numbers>
#include <stdexcept>
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

    virtual void translate(Vec2) = 0;
    virtual void rotate(double) = 0;
    virtual void mirror(Vec2) = 0;
}; // struct Shape


struct SimpleShape : public Shape {
    SimpleShape()
        : position { Vec2::ZERO() }
        , rotation { 0 }
    { }

    void translate(Vec2 translation)
    {
        position += translation;
    }

    void rotate(double angle)
    {
        position = rotate2D(Vec2::ZERO(), position, angle);
        rotation += angle;
    }

    /**
     * Works for non-chiral shapes
     * param: mirror_plane unit-length vector perpendicular to the mirror
     */
    void mirror(Vec2 mirror_plane)
    {
        double plane_angle = angle2D(mirror_plane);
        rotation = std::numbers::pi + 2 * plane_angle - rotation;
        Vec2 dist_to_mirror = mirror_plane * position.dot(mirror_plane);
        Vec2 length_along_mirror = position - dist_to_mirror;
        position = -dist_to_mirror + length_along_mirror;
    }

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

    void mirror(Vec2 mirror_plane)
    {
        throw std::logic_error("Not implemented");
    }

    std::vector<Vec2> vertices;
}; // struct Polygon


struct ShapeContainer : public Shape {
    void translate(Vec2 translation)
    {
        for (auto& child : children)
            child->translate(translation);
    }

    void rotate(double angle)
    {
        for (auto& child : children)
            child->rotate(angle);
    }

    void mirror(Vec2 mirror_plane)
    {
        for (auto& child : children)
            child->mirror(mirror_plane);
    }

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
