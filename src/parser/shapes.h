#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "base/geometry.h"
#include "parser/Parseable.h"
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


class Shape : public Parseable {
  public:
    virtual ShapeTypeId type_id() = 0;

    virtual void translate(Vec2) = 0;
    virtual void rotate(double) = 0;
    virtual void mirror(Vec2) = 0;

    // virtual void print() = 0;
}; // class Shape


class SimpleShape : public Shape {
  public:
    SimpleShape(Vec2 position = Vec2::ZERO(), double rotation = 0)
        : position { position }
        , rotation { rotation }
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
    void mirror(Vec2 mirror_plane);

    Vec2 position;
    double rotation;
}; // class SimpleShape

class Rectangle : public SimpleShape {
  public:
    Rectangle(Vec2 size, bool center)
        : SimpleShape { (-size / 2) * center }
        , size { size }
    { }

    Rectangle(double x, double y, bool center)
        : SimpleShape { (-Vec2(x, y) / 2) * center }
        , size { x, y }
    { }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Rectangle;
    }

    void mirror(Vec2 mirror_plane);

    void _normalize();

    void print()
    {
        printf(
            "Rectangle: %f, %f; (%f, %f / %f)\n",
            size.x,
            size.y,
            position.x,
            position.y,
            rotation
        );
    }

    Vec2 size;
}; // class Rectangle

class Circle : public SimpleShape {
  public:
    Circle(double radius)
        : radius { radius }
    { }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Circle;
    }

    virtual void print()
    {
        printf(
            "Circle: %f; (%f, %f / %f)\n",
            radius,
            position.x,
            position.y,
            rotation
        );
    }

    double radius;
}; // class Circle

class Polygon : public SimpleShape {
  public:
    Polygon(const std::vector<Vec2>& vertices)
        : vertices { vertices }
    { }

    Polygon(const std::vector<double>& numbers)
    {
        if (numbers.size() % 2 == 1)
            throw std::invalid_argument(
                "Required even number of half-coordinates"
            );
        for (size_t i = 0; i < numbers.size(); i += 2)
            vertices.emplace_back(numbers[i], numbers[i + 1]);
    }

    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Polygon;
    }

    void mirror(Vec2 mirror_plane)
    {
        throw std::logic_error("Not implemented");
    }

    virtual void print()
    {
        printf("Polygon: ");
        for (Vec2 vec : vertices)
            printf("[%f, %f], ", vec.x, vec.y);
        printf("\b (%f, %f / %f)\n", position.x, position.y, rotation);
    }

    std::vector<Vec2> vertices;
}; // class Polygon


class ShapeContainer : public Shape {
  public:
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
}; // class ShapeContainer

class Union : public ShapeContainer {
  public:
    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Union;
    }

    void print()
    {
        printf("Union:\n");
        for (auto& shape : children)
            shape->print();
        printf("/union\n");
    }
}; // class Union

class Difference : public ShapeContainer {
  public:
    virtual ShapeTypeId type_id()
    {
        return ShapeTypeId::Difference;
    }

    void print()
    {
        printf("Difference:\n");
        for (auto& shape : children)
            shape->print();
        printf("/difference\n");
    }
}; // class Difference

} // namespace packer

#endif // SHAPES_H_INCLUDED
