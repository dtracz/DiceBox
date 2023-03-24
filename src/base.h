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


struct Color : Vec3 {
    double alpha;

    Color(double r=1, double g=1, double b=1, double alpha=1):
        Vec3 {r, g, b}, alpha {alpha} { }

    bool is_valid() {
        return std::min({x,y,z,alpha}) >= 0
            && std::max({x,y,z,alpha}) <= 1;
    }
};


class Module3D;

class Part3D {
  public:
    virtual void set_color(Color) = 0;

    virtual Part3D& translate(Vec3) = 0;

    virtual Part3D& rotate(Vec3) = 0;

    virtual Part3D& rotate(Vec3, Vec3) = 0;

    virtual Part3D& mirror(Vec3) = 0;

    virtual Part3D& mirror(Vec3, Vec3) = 0;

    virtual void render3D(IndentWriter&) = 0;

  protected:
    virtual Component _get_final_form() = 0;

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

    template<typename T>
        requires std::convertible_to<T, Component2D>
    FlatPart(T&& comp, double thickness=-1, Color color = {-1}):
            _shape{std::forward<T>(comp)},
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


    FlatPart& translate(Vec3 vec) override;

    FlatPart& rotate(Vec3 vec) override {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tRotate, -vec);
        return *this;
    }

    FlatPart& rotate(Vec3 vec, Vec3 center) override;

    FlatPart& mirror(Vec3 vec) override {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tMirror, -vec);
        return *this;
    }

    FlatPart& mirror(Vec3 vec, Vec3 center) override;


    void render2D(IndentWriter& writer) const {
        writer << _shape;
    }


    Component _get_final_form() override;

    void render3D(IndentWriter& writer) override {
        writer << _get_final_form();
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
    template<typename T>
        requires std::convertible_to<T, Component2D>
    HelperPart(T&& other): Component {std::forward<T>(other)} { }

    void set_color(Color color) override {
        Component::color(color.x, color.y, color.z, color.alpha);
    }

    HelperPart& translate(Vec3 vec) override {
        Component::translate(vec.x, vec.y, vec.z);
        return *this;
    }

    HelperPart& rotate(Vec3 vec) override {
        Component::rotate(vec.x, vec.y, vec.z);
        return *this;
    }

    HelperPart& rotate(Vec3, Vec3) override;

    HelperPart& mirror(Vec3 vec) override {
        Component::mirror(vec.x, vec.y, vec.z);
        return *this;
    }

    HelperPart& mirror(Vec3, Vec3) override;

    void render3D(IndentWriter& writer) override {
        writer << *this;
    }

  protected:
    Component _get_final_form() override {
        return *this;
    }

};  // class HelperPart


class Module3D : public Part3D {
    enum class _CompositionT {
        tAdd,
        tCut,
    };

  public:
    // TODO: deep-copy constructor
    // Module3D(Module3D& other);

    template <typename T1, typename T2>
        requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
              && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    static Module3D Union(T1&& part1, T2&& part2) {
        return Module3D(
                std::forward<T1>(part1),
                std::forward<T2>(part2),
                _CompositionT::tAdd
        );
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
        for (auto pair : _parts)
            pair.second->set_color(color);
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

    Module3D& rotate(Vec3, Vec3) override;

    Module3D& mirror(Vec3 vec) override {
        for (auto pair : _parts)
            pair.second->mirror(vec);
        return *this;
    }

    Module3D& mirror(Vec3, Vec3) override;

    void render3D(IndentWriter& writer) override {
        writer << _get_final_form();
    }

  protected:
    Component _get_final_form() override;

  private:
    std::vector<std::pair<
        _CompositionT,
        std::shared_ptr<Part3D>>
    > _parts;


    template <typename T1, typename T2>
        requires std::derived_from<T1, Part3D>
              && std::derived_from<T2, Part3D>
    Module3D (T1 part1, T2 part2, _CompositionT ctype) {
        auto shp1 = std::shared_ptr<Part3D>(new T1(std::move(part1)));
        _parts.emplace_back(_CompositionT::tAdd, shp1);
        auto shp2 = std::shared_ptr<Part3D>(new T2(std::move(part2)));
        _parts.emplace_back(ctype, shp2);
    }

};  // class Module3D


#endif  // BASE_H_INCLUDED
