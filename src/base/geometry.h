#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <cmath>
#include <numbers>


constexpr double _EPS = 1e-8;


struct Vec2 {
    double x, y;

    constexpr Vec2(double x, double y)
        : x { x }
        , y { y }
    { }

    constexpr static const Vec2 ZERO()
    {
        return { 0, 0 };
    }

    constexpr Vec2 operator-() const
    {
        return { -x, -y };
    }

    constexpr Vec2 operator+(Vec2 other) const
    {
        return { x + other.x, y + other.y };
    }

    constexpr Vec2 operator*(double value) const
    {
        return { x * value, y * value };
    }

    constexpr double dot(Vec2 other) const
    {
        return x * other.x + y * other.y;
    }

    constexpr Vec2 operator/(double value) const
    {
        return { x / value, y / value };
    }

    constexpr Vec2& operator*=(double value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    constexpr Vec2& operator+=(Vec2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2 operator-(Vec2 other) const
    {
        return { x - other.x, y - other.y };
    }

    constexpr Vec2& operator-=(Vec2 other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr bool operator==(Vec2 other) const
    {
        return std::abs(x - other.x) < _EPS && std::abs(y - other.y) < _EPS;
    }

    constexpr double length() const
    {
        return std::sqrt(x * x + y * y);
    }

}; // struct Vec2


struct Vec3 {
    double x, y, z;

    constexpr Vec3(double x, double y, double z)
        : x { x }
        , y { y }
        , z { z }
    { }

    constexpr static const Vec3 ZERO()
    {
        return { 0, 0, 0 };
    }

    constexpr static Vec3 fromXY(Vec2 vec)
    {
        return { vec.x, vec.y, 0 };
    }

    constexpr static Vec3 fromXZ(Vec2 vec)
    {
        return { vec.x, 0, vec.y };
    }

    constexpr static Vec3 fromYZ(Vec2 vec)
    {
        return { 0, vec.x, vec.y };
    }

    constexpr Vec3 operator-() const
    {
        return { -x, -y, -z };
    }

    constexpr Vec3 operator+(Vec3 other) const
    {
        return { x + other.x, y + other.y, z + other.z };
    }

    constexpr Vec3 operator*(double value) const
    {
        return { x * value, y * value, z * value };
    }

    constexpr Vec3 operator/(double value) const
    {
        return { x / value, y / value, z / value };
    }

    constexpr Vec3& operator*=(double value)
    {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    constexpr Vec3& operator+=(Vec3 other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vec3 operator-(Vec3 other) const
    {
        return { x - other.x, y - other.y, z - other.z };
    }

    constexpr Vec3& operator-=(Vec3 other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr bool operator==(Vec3 other) const
    {
        return std::abs(x - other.x) < _EPS && std::abs(y - other.y) < _EPS
            && std::abs(z - other.z) < _EPS;
    }

    constexpr double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

}; // struct Vec3



inline constexpr double sqr(double x)
{
    return x * x;
}

inline double mod(double x, double m) {
    x = std::fmod(x, m);
    return x + ((x < 0) * m);
}

inline double angle2D(double x, double y)
{
    double angle = std::atan2(y, x);
    return mod(angle, 2*std::numbers::pi);
}

inline double angle2D(Vec2 vec)
{
    double angle = std::atan2(vec.y, vec.x);
    return mod(angle, 2*std::numbers::pi);
}

Vec2 rotate2D(Vec2 axis, Vec2 point, double angle);

inline Vec2 rotate2D(Vec2 point, double angle)
{
    return rotate2D(Vec2::ZERO(), point, angle);
}

inline double mirror(double plane_angle, double angle)
{
    return 2 * plane_angle - angle;
}

Vec2 mirror(Vec2 point, Vec2 mirror_plane);


#endif // GEOMETRY_H_INCLUDED
