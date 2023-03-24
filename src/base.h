#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdexcept>
#include <vector>
#include <utility>
#include <concepts>
#include "core/Component2D.h"


struct Vec3 {
    double x, y, z;

    Vec3(double x, double y, double z): x{x}, y{y}, z{z} { }

    static const Vec3 ZERO() {
        return {0, 0, 0};
    }

    Vec3 operator-() const {
        return {-x, -y, -z};
    }

    Vec3 operator+(Vec3 other) const {
        return {x+other.x, y+other.y, z+other.z};
    }

    Vec3& operator+=(Vec3 other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 operator-(Vec3 other) const {
        return {x-other.x, y-other.y, z-other.z};
    }

    Vec3& operator-=(Vec3 other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    bool operator==(Vec3 other) const;

};  // class Vec3



class FlatPart {
    enum class _TransformT {
        tTranslate,
        tRotate,
        tMirror,
    };

  public:
    FlatPart(double thickness=-1):
            _shape{Component2D()},
            _empty{true},
            _thickness{thickness} { }

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart(T&& comp, double thickness=-1):
            _shape{std::forward<T>(comp)},
            _empty{false},
            _thickness{thickness} { }


    void set_thickness(double thickness, bool force_reset=false) {
        if (_thickness > 0 && !force_reset)
            throw std::runtime_error("thichness is already set");
        _thickness = thickness;
    }


    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart& add(T&& shape) {
        if (_empty) {
            _shape = std::forward<T>(shape);
            _empty = false;
        } else {
            _shape = _shape + std::forward<T>(shape);
        }
        return *this;
    }

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart& operator+(T&& shape) {
        return this->add(std::forward<T>(shape));
    }

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart& cut(T&& shape) {
        if (_empty)
            throw std::runtime_error("cannot cut from empty component");
        _shape = _shape - std::forward<T>(shape);
        return *this;
    }

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart& operator-(T&& shape) {
        return this->cut(std::forward<T>(shape));
    }


    FlatPart& translate(Vec3 vec);

    FlatPart& rotate(Vec3 vec) {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tRotate, -vec);
        return *this;
    }

    FlatPart& rotate(Vec3 vec, Vec3 center);

    FlatPart& mirror(Vec3 vec) {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tMirror, -vec);
        return *this;
    }

    FlatPart& mirror(Vec3 vec, Vec3 center);


    void render2D(IndentWriter& writer) const {
        writer << _shape;
    }

    void render3D(IndentWriter&);


  private:
    Component2D _shape;
    bool _empty;
    double _thickness;
    std::vector<std::pair<_TransformT, Vec3>> _transforms;

};  // class FlatPart


#endif  // BASE_H_INCLUDED
