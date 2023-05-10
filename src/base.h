#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <stdexcept>
#include <vector>
#include <utility>
#include <memory>
#include <concepts>
#include <type_traits>
#include "core/Component2D.h"
#include "core/Component.h"


struct Vec2 {
    double x, y;

    constexpr Vec2(double x, double y): x{x}, y{y} { }

    constexpr static const Vec2 ZERO() {
        return {0, 0};
    }

    constexpr Vec2 operator-() const {
        return {-x, -y};
    }

    constexpr Vec2 operator+(Vec2 other) const {
        return {x+other.x, y+other.y};
    }

    constexpr Vec2 operator+(double other) const {
        return {x+other, y+other};
    }

    constexpr Vec2 operator*(double other) const {
        return {x*other, y*other};
    }

    constexpr Vec2 operator/(double other) const {
        return {x/other, y/other};
    }

    constexpr Vec2& operator*=(double value) {
        x *= value;
        y *= value;
        return *this;
    }

    constexpr Vec2& operator+=(Vec2 other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2 operator-(Vec2 other) const {
        return {x-other.x, y-other.y};
    }

    constexpr Vec2& operator-=(Vec2 other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr bool operator==(Vec2 other) const;

    constexpr double length() const {
        return std::sqrt(x*x + y*y);
    }

};  // struct Vec2


struct Vec3 {
    double x, y, z;

    constexpr Vec3(double x, double y, double z): x{x}, y{y}, z{z} { }

    constexpr static const Vec3 ZERO() {
        return {0, 0, 0};
    }

    constexpr static Vec3 fromXY(Vec2 vec) {
        return {vec.x, vec.y, 0};
    }

    constexpr static Vec3 fromXZ(Vec2 vec) {
        return {vec.x, 0, vec.y};
    }

    constexpr static Vec3 fromYZ(Vec2 vec) {
        return {0, vec.x, vec.y};
    }

    constexpr Vec3 operator-() const {
        return {-x, -y, -z};
    }

    constexpr Vec3 operator+(Vec3 other) const {
        return {x+other.x, y+other.y, z+other.z};
    }

    constexpr Vec3 operator+(double other) const {
        return {x+other, y+other, z+other};
    }

    constexpr Vec3 operator*(double other) const {
        return {x*other, y*other, z*other};
    }

    constexpr Vec3 operator/(double other) const {
        return {x/other, y/other, z/other};
    }

    constexpr Vec3& operator*=(double value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    constexpr Vec3& operator+=(Vec3 other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vec3 operator-(Vec3 other) const {
        return {x-other.x, y-other.y, z-other.z};
    }

    constexpr Vec3& operator-=(Vec3 other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr bool operator==(Vec3 other) const;

    constexpr double length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

};  // struct Vec3


struct Color : Vec3 {
    double alpha;

    Color(double r=1, double g=1, double b=1, double alpha=1):
        Vec3 {r, g, b}, alpha {alpha} { }

    bool is_valid() {
        return std::min({x,y,z,alpha}) >= 0
            && std::max({x,y,z,alpha}) <= 1;
    }
};


class IColorGenerator {
  public:
    virtual Color next() = 0;
    virtual Color operator[](size_t idx) const = 0;
};


class Module3D;

class Part3D {
  public:
    virtual void set_color(Color) = 0;

    virtual Part3D& translate(Vec3) = 0;

    virtual Part3D& rotate(Vec3) = 0;

    virtual Part3D& rotate(Vec3, Vec3);

    virtual Part3D& mirror(Vec3) = 0;

    virtual Part3D& mirror(Vec3, Vec3);

    virtual void render3D(IndentWriter&) = 0;

  protected:
    virtual Component _get_final_form() = 0;

    virtual std::shared_ptr<Part3D> _clone() = 0;

    friend class Module3D;
};


class FlatPart : public Part3D {
    enum class _TransformT {
        tTranslate,
        tRotate,
        tMirror,
    };

  public:
    FlatPart(double thickness=-1, Color color = {-1}):
            _shape{Component2D()},
            _empty{true},
            _thickness{thickness},
            _color{color} { }

    FlatPart(std::convertible_to<Component2D> auto&& comp,
             double thickness=-1, Color color = {-1}):
            _shape{std::forward<decltype(comp)>(comp)},
            _empty{false},
            _thickness{thickness},
            _color{color} { }


    void set_thickness(double thickness, bool force_reset=false) {
        if (_thickness > 0 && !force_reset)
            throw std::runtime_error("thichness is already set");
        _thickness = thickness;
    }

    void set_color(Color color) override {
        _color = color;
    }


    FlatPart& add(std::convertible_to<Component2D> auto&& shape) {
        if (_empty) {
            _shape = std::forward<decltype(shape)>(shape);
            _empty = false;
        } else {
            _shape = _shape + std::forward<decltype(shape)>(shape);
        }
        return *this;
    }

    FlatPart& operator+=(std::convertible_to<Component2D> auto&& shape) {
        return this->add(std::forward<decltype(shape)>(shape));
    }

    FlatPart& cut(std::convertible_to<Component2D> auto&& shape) {
        if (_empty)
            throw std::runtime_error("cannot cut from empty component");
        _shape = _shape - std::forward<decltype(shape)>(shape);
        return *this;
    }

    FlatPart& operator-=(std::convertible_to<Component2D> auto&& shape) {
        return this->cut(std::forward<decltype(shape)>(shape));
    }


    FlatPart& translate(Vec3) override;

    FlatPart& rotate(Vec3 vec) override {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tRotate, vec);
        return *this;
    }

    using Part3D::rotate;

    FlatPart& mirror(Vec3 vec) override {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tMirror, vec);
        return *this;
    }

    using Part3D::mirror;


    void render2D(IndentWriter& writer) const {
        writer << _shape;
    }


    void render3D(IndentWriter& writer) override {
        writer << _get_final_form();
    }

  protected:
    Component _get_final_form() override;

    std::shared_ptr<Part3D> _clone() override {
        return std::make_shared<FlatPart>(*this);
    }

  private:
    Component2D _shape;
    bool _empty;
    double _thickness;
    Color _color;
    std::vector<std::pair<_TransformT, Vec3>> _transforms;

};  // class FlatPart


class HelperPart : public Part3D,
                   public Component {
  public:
    HelperPart(std::convertible_to<Component2D> auto&& other):
            Component {std::forward<decltype(other)>(other)} { }

    void set_color(Color color) override {
        Component::color(color.x, color.y, color.z, color.alpha);
    }

    HelperPart& translate(Vec3 vec) override {
        Component::translate(vec.x, vec.y, vec.z);
        return *this;
    }

    HelperPart& rotate(Vec3 vec) override {
        Component::rotate(-vec.x, -vec.y, -vec.z);
        return *this;
    }

    using Part3D::rotate;

    HelperPart& mirror(Vec3 vec) override {
        Component::mirror(vec.x, vec.y, vec.z);
        return *this;
    }

    using Part3D::mirror;

    void render3D(IndentWriter& writer) override {
        writer << *this;
    }

  protected:
    Component _get_final_form() override {
        return *this;
    }

    std::shared_ptr<Part3D> _clone() override {
        return std::make_shared<HelperPart>(*this);
    }

};  // class HelperPart


class Module3D : public Part3D {
    enum class _CompositionT {
        tAdd,
        tCut,
    };

  public:
    template <typename T>
        requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D(T&& part) {
        _append(std::forward<T>(part), _CompositionT::tAdd);
    }

    Module3D(Module3D&);
    Module3D(Module3D&&) = default;

    static Module3D Union(std::derived_from<Part3D> auto... args) {
        return (... + args);
    }

    template <typename T1, typename T2>
        requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
              && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    static Module3D Difference(T1&& part1, T2&& part2) {
        return Module3D(
                std::forward<T1>(part1),
                std::forward<T2>(part2),
                _CompositionT::tCut
        );
    }

    void set_color(Color color) override {
        _color = color;
    }

    Module3D& translate(Vec3 vec) override {
        for (auto pair : _parts)
            pair.second->translate(vec);
        return *this;
    }

    Module3D& rotate(Vec3 vec) override {
        for (auto pair : _parts)
            pair.second->rotate(vec);
        return *this;
    }

    using Part3D::rotate;

    Module3D& mirror(Vec3 vec) override {
        for (auto pair : _parts)
            pair.second->mirror(vec);
        return *this;
    }

    using Part3D::mirror;

    void render3D(IndentWriter& writer) override {
        writer << _get_final_form();
    }


    template <typename T>
        requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& operator+=(T&& part) {
        return _append(std::forward<T>(part), _CompositionT::tAdd);
    }

    template <typename T>
        requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& operator-=(T&& part) {
        return _append(std::forward<T>(part), _CompositionT::tCut);
    }


  protected:
    Component _get_final_form() override;

    std::shared_ptr<Part3D> _clone() override {
        return std::make_shared<Module3D>(*this);
    }

  private:
    std::vector<std::pair<
        _CompositionT,
        std::shared_ptr<Part3D>>
    > _parts;

    Color _color = {-1,0,0};


    template <typename T1, typename T2>
        requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
              && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    Module3D(T1&& part1, T2&& part2, _CompositionT ctype) {
        _append(std::forward<T1>(part1), _CompositionT::tAdd);
        _append(std::forward<T2>(part2), ctype);
    }

    template <typename T>
        requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& _append(T&& part, _CompositionT ctype) {
        auto shp = std::make_shared<typename std::remove_reference<T>::type>(
                std::forward<T>(part)
        );
        _parts.emplace_back(ctype, shp);
        return *this;
    }

    template <typename T1, typename T2>
        requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
              && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    friend Module3D operator+(T1&&, T2&&);

    template <typename T1, typename T2>
        requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
              && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    friend Module3D operator-(T1&&, T2&&);

};  // class Module3D



template <typename T1, typename T2>
    requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
          && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
Module3D operator+(T1&& part1, T2&& part2) {
    if (std::same_as<typename std::remove_reference<T1>::type, Module3D>) {
        Module3D mod {std::forward<T1>(part1)};
        return mod._append(std::forward<T2>(part2), Module3D::_CompositionT::tAdd);
    } else {
        return Module3D(
                std::forward<T1>(part1),
                std::forward<T2>(part2),
                Module3D::_CompositionT::tAdd
        );
    }
}


template <typename T1, typename T2>
    requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
          && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
Module3D operator-(T1&& part1, T2&& part2) {
    return Module3D(
            std::forward<T1>(part1),
            std::forward<T2>(part2),
            Module3D::_CompositionT::tCut
    );
}


#endif  // BASE_H_INCLUDED
