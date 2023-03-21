#include <iostream>
#include <iomanip>
#include <utility>
#include <concepts>
#include <vector>
#include <array>
#include "core.h"
#include "components.h"


class FlatPart {
    enum class _TransformT {
        tTranslate,
        tRotate,
    };

  private:
    Component2D _shape;
    bool _empty;
    double _thickness;
    std::vector<std::pair<_TransformT, std::array<double, 3>>> _transforms;


    static inline std::array<double, 3> _negate(std::array<double, 3> v) {
        return {-v[0], -v[1], -v[2]};
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


    FlatPart& translate(std::array<double,3> vec) {
        _transforms.emplace_back(_TransformT::tTranslate, vec);
        return *this;
    }


    FlatPart& rotate(std::array<double,3> vec) {
        _transforms.emplace_back(_TransformT::tRotate, vec);
        return *this;
    }


    FlatPart& rotate(std::array<double,3> vec, std::array<double,3> center) {
        this->translate(_negate(center));
        this->rotate(vec);
        this->translate(center);
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
                    part.translate(transform_vec[0],
                                   transform_vec[1],
                                   transform_vec[2]);
                    break;
                case _TransformT::tRotate:
                    part.rotate(transform_vec[0],
                                transform_vec[1],
                                transform_vec[2]);
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

    fp.translate({1,2,3});

    fp.set_thickness(3);

    IndentWriter writer;
    fp.render3D(writer);
    std::cout << writer;

    return 0;
}

