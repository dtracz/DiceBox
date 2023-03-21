#include <iostream>
#include <iomanip>
#include <utility>
#include <concepts>
#include <vector>
#include <cmath>
#include "core.h"
#include "components.h"


const double EPS = 1e-8;


struct Vec3 {
    double x, y, z;

    Vec3(double x, double y, double z): x{x}, y{y}, z{z} { }

    static const Vec3 ZERO() {
        return {0, 0, 0};
    }

    Vec3 operator-() {
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

    bool operator==(Vec3 other) const {
        return std::abs(x - other.x) < EPS
            && std::abs(y - other.y) < EPS
            && std::abs(z - other.z) < EPS;
    }
};


class FlatPart {
  private:
    enum class _TransformT {
        tTranslate,
        tRotate,
    };

    Component2D _shape;
    bool _empty;
    double _thickness;
    std::vector<std::pair<_TransformT, Vec3>> _transforms;

    FlatPart& _transform(_TransformT type, Vec3 vec) {
        if (!_transforms.empty() &&
             _transforms.back().first == type) {
            auto pair = _transforms.back();
            _transforms.pop_back();
            Vec3 prev_transform = pair.second;
            vec += prev_transform;
        }
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tTranslate, vec);
        return *this;
    }


  public:
    FlatPart(double thickness=-1):
            _shape{Component2D()},
            _empty{true},
            _thickness{thickness} {
    }

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart(T&& comp, double thickness=-1):
            _shape{std::forward<T>(comp)},
            _empty{false},
            _thickness{thickness} {
    }


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


    FlatPart& translate(Vec3 vec) {
        return _transform(_TransformT::tTranslate, vec);
    }

    FlatPart& rotate(Vec3 vec) {
        return _transform(_TransformT::tRotate, vec);
    }

    FlatPart& rotate(Vec3 vec, Vec3 center) {
        translate(-center);
        rotate(vec);
        translate(center);
        return *this;
    }


    void render2D(IndentWriter& writer) {
        writer << _shape;
    }

    void render3D(IndentWriter& writer) {
        if (_thickness < 0)
            throw std::runtime_error("set thickness first");
        auto part = _shape.linearExtrudedCopy(_thickness);
        for (const auto& t : _transforms) {
            auto transform_type = t.first;
            auto transform_vec = t.second;
            switch (transform_type) {
                case _TransformT::tTranslate:
                    part.translate(transform_vec.x,
                                   transform_vec.y,
                                   transform_vec.z);
                    break;
                case _TransformT::tRotate:
                    part.rotate(transform_vec.x,
                                transform_vec.y,
                                transform_vec.z);
                    break;
            }
        }
        writer << part;
    }

};



int main() {
    FlatPart fp{Circle::create(4)};
    fp + Circle::create(3).translate(2,1,0)
       - Circle::create(2)
       + Square::create(1, 10);

    auto fp2 = fp;

    fp.translate({1,0,3});
    fp.translate({0,2,0});
    fp.set_thickness(3);

    fp2.translate({0,0,-2});
    fp2.set_thickness(2);

    IndentWriter writer;
    fp.render3D(writer);
    fp2.render3D(writer);
    std::cout << writer;

    return 0;
}

