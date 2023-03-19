#include <iostream>
#include <iomanip>
#include <utility>
#include <concepts>
#include "core.h"
#include "components.h"


class FlatPart {
  private:
    Component2D _shape;
    bool _empty;
    double _thickness;

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


    void render2D(IndentWriter& writer) {
        writer << _shape;
    }

};



int main() {
    FlatPart fp{Circle::create(4)};
    fp + Circle::create(3).translate(2,1,0)
       - Circle::create(2)
       + Square::create(1, 10);

    IndentWriter writer;
    fp.render2D(writer);
    std::cout << writer;

    return 0;
}

