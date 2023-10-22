#include "parser/shapes.h"
#include "base/geometry.h"
#include <cmath>
#include <numbers>

using namespace parser;

void SimpleShape::mirror(Vec2 mirror_plane)
{
    double plane_angle = angle2D(mirror_plane);
    rotation = std::numbers::pi + 2 * plane_angle - rotation;
    Vec2 dist_to_mirror = mirror_plane * position.dot(mirror_plane);
    Vec2 length_along_mirror = position - dist_to_mirror;
    position = -dist_to_mirror + length_along_mirror;
}


void Rectangle::mirror(Vec2 mirror_plane)
{
    position = ::mirror(position, mirror_plane);
    Vec2 real_diagonal = rotate2D(size, rotation);
    real_diagonal = ::mirror(real_diagonal, mirror_plane);
    rotation = 2 * angle2D(mirror_plane) - rotation;
    size = rotate2D(real_diagonal, -rotation);
    _normalize();
}

void Rectangle::_normalize()
{
    if (size.x < 0) {
        position += rotate2D({ size.x, 0 }, rotation);
        size.x = -size.x;
    }
    if (size.y < 0) {
        position += rotate2D({ 0, size.y }, rotation);
        size.y = -size.y;
    }
}

std::vector<Vec2> Rectangle::get_hull()
{
    double north_bound = position.y;
    double south_bound = position.y;
    double east_bound = position.x;
    double west_bound = position.x;
    Vec2 vertices[] = {
        rotate2D(position, position + Vec2(size.x, 0), rotation),
        rotate2D(position, position + Vec2(0, size.y), rotation),
        rotate2D(position, position + size, rotation),
    };
    for (int i = 0; i < 3; i++) {
        north_bound = std::max(north_bound, vertices[i].y);
        south_bound = std::min(south_bound, vertices[i].y);
        east_bound = std::max(east_bound, vertices[i].x);
        west_bound = std::min(west_bound, vertices[i].x);
    }
    return {
        { east_bound, north_bound },
        { west_bound, north_bound },
        { west_bound, south_bound },
        { east_bound, south_bound },
    };
}


std::vector<Vec2> Circle::get_hull()
{
    double north_bound = position.y + radius;
    double south_bound = position.y - radius;
    double east_bound = position.x + radius;
    double west_bound = position.x - radius;
    return {
        { east_bound, north_bound },
        { west_bound, north_bound },
        { west_bound, south_bound },
        { east_bound, south_bound },
    };
}


void Polygon::mirror(Vec2 mirror_plane)
{
    position = ::mirror(position, mirror_plane);
    rotation = 2 * angle2D(mirror_plane) - rotation;
    for (Vec2& vertex : vertices)
        vertex = ::mirror(vertex, mirror_plane);
}

std::vector<Vec2> Polygon::get_hull()
{
    auto vertex = rotate2D(Vec2::ZERO(), vertices[0], rotation);
    double north_bound = vertex.y + position.y;
    double south_bound = vertex.y + position.y;
    double east_bound = vertex.x + position.x;
    double west_bound = vertex.x + position.x;
    for (int i = 1; i < vertices.size(); i++) {
        auto vertex = rotate2D(Vec2::ZERO(), vertices[i], rotation);
        north_bound = std::max(north_bound, vertex.y + position.y);
        south_bound = std::min(south_bound, vertex.y + position.y);
        east_bound = std::max(east_bound, vertex.x + position.x);
        west_bound = std::min(west_bound, vertex.x + position.x);
    }
    return {
        { east_bound, north_bound },
        { west_bound, north_bound },
        { west_bound, south_bound },
        { east_bound, south_bound },
    };
}


std::vector<Vec2> Union::get_hull()
{
    std::vector<Vec2> hull = children[0]->get_hull();
    for (int i = 1; i < children.size(); i++) {
        std::vector<Vec2> childs_hull = children[i]->get_hull();
        hull[0].y = std::max(hull[0].y, childs_hull[0].y);
        hull[2].y = std::min(hull[2].y, childs_hull[2].y);
        hull[0].x = std::max(hull[0].x, childs_hull[0].x);
        hull[2].x = std::min(hull[2].x, childs_hull[2].x);
    }
    hull[1] = { hull[2].x, hull[0].y };
    hull[3] = { hull[0].x, hull[2].y };
    return hull;
}


std::vector<Vec2> Difference::get_hull()
{
    return children[0]->get_hull();
}
