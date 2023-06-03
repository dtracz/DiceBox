#ifndef PARTS_HPP_INCLUDED
#define PARTS_HPP_INCLUDED

#include <concepts>
#include <list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "base/colors.h"
#include "base/geometry.h"
#include "core/Component.h"
#include "core/Component2D.h"

class Module3D;
class FlatPart;

class Part3D {
  public:
    virtual void set_color(Color) = 0;

    virtual Part3D& translate(Vec3) = 0;

    virtual Part3D& rotate(Vec3) = 0;

    virtual Part3D& rotate(Vec3, Vec3);

    virtual Part3D& mirror(Vec3) = 0;

    virtual Part3D& mirror(Vec3, Vec3);

    virtual void render3D(IndentWriter&) = 0;

    virtual std::list<FlatPart> get_all_flats() const = 0;

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
    FlatPart(double thickness = -1, Color color = { -1 })
        : _shape { Component2D() }
        , _empty { true }
        , _thickness { thickness }
        , _color { color }
    { }

    FlatPart(
        std::convertible_to<Component2D> auto&& comp,
        double thickness = -1,
        Color color = { -1 }
    )
        : _shape { std::forward<decltype(comp)>(comp) }
        , _empty { false }
        , _thickness { thickness }
        , _color { color }
    { }

    void set_thickness(double thickness, bool force_reset = false)
    {
        if (_thickness > 0 && !force_reset)
            throw std::runtime_error("thichness is already set");
        _thickness = thickness;
    }

    double get_thickness() const
    {
        return _thickness;
    }

    void set_color(Color color) override
    {
        _color = color;
    }

    FlatPart& add(std::convertible_to<Component2D> auto&& shape)
    {
        if (_empty) {
            _shape = std::forward<decltype(shape)>(shape);
            _empty = false;
        } else {
            _shape = _shape + std::forward<decltype(shape)>(shape);
        }
        return *this;
    }

    FlatPart& operator+=(std::convertible_to<Component2D> auto&& shape)
    {
        return this->add(std::forward<decltype(shape)>(shape));
    }

    FlatPart& cut(std::convertible_to<Component2D> auto&& shape)
    {
        if (_empty)
            throw std::runtime_error("cannot cut from empty component");
        _shape = _shape - std::forward<decltype(shape)>(shape);
        return *this;
    }

    FlatPart& operator-=(std::convertible_to<Component2D> auto&& shape)
    {
        return this->cut(std::forward<decltype(shape)>(shape));
    }

    FlatPart& translate(Vec3) override;

    FlatPart& rotate(Vec3 vec) override
    {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tRotate, vec);
        return *this;
    }

    using Part3D::rotate;

    FlatPart& mirror(Vec3 vec) override
    {
        if (vec != Vec3::ZERO())
            _transforms.emplace_back(_TransformT::tMirror, vec);
        return *this;
    }

    using Part3D::mirror;

    void render2D(IndentWriter& writer) const
    {
        writer << _shape;
    }

    void render3D(IndentWriter& writer) override
    {
        writer << _get_final_form();
    }

    std::list<FlatPart> get_all_flats() const override
    {
        return { *this };
    }

  protected:
    Component _get_final_form() override;

    std::shared_ptr<Part3D> _clone() override
    {
        return std::make_shared<FlatPart>(*this);
    }

  private:
    Component2D _shape;
    bool _empty;
    double _thickness;
    Color _color;
    std::vector<std::pair<_TransformT, Vec3>> _transforms;

}; // class FlatPart

class HelperPart : public Part3D,
                   public Component {
  public:
    HelperPart(std::convertible_to<Component2D> auto&& other)
        : Component { std::forward<decltype(other)>(other) }
    { }

    void set_color(Color color) override
    {
        Component::color(color.x, color.y, color.z, color.alpha);
    }

    HelperPart& translate(Vec3 vec) override
    {
        Component::translate(vec.x, vec.y, vec.z);
        return *this;
    }

    HelperPart& rotate(Vec3 vec) override
    {
        Component::rotate(-vec.x, -vec.y, -vec.z);
        return *this;
    }

    using Part3D::rotate;

    HelperPart& mirror(Vec3 vec) override
    {
        Component::mirror(vec.x, vec.y, vec.z);
        return *this;
    }

    using Part3D::mirror;

    void render3D(IndentWriter& writer) override
    {
        writer << *this;
    }

    std::list<FlatPart> get_all_flats() const override
    {
        return {};
    }

  protected:
    Component _get_final_form() override
    {
        return *this;
    }

    std::shared_ptr<Part3D> _clone() override
    {
        return std::make_shared<HelperPart>(*this);
    }

}; // class HelperPart

class Module3D : public Part3D {
    enum class _CompositionT {
        tAdd,
        tCut,
    };

  public:
    template <typename T>
    requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D(T&& part)
    {
        _append(std::forward<T>(part), _CompositionT::tAdd);
    }

    Module3D(Module3D&);
    Module3D(Module3D&&) = default;

    static Module3D Union(std::derived_from<Part3D> auto... args)
    {
        return (... + args);
    }

    template <typename T1, typename T2>
    requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
          && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    static Module3D Difference(T1&& part1, T2&& part2)
    {
        return Module3D(
            std::forward<T1>(part1),
            std::forward<T2>(part2),
            _CompositionT::tCut
        );
    }

    void set_color(Color color) override
    {
        _color = color;
    }

    Module3D& translate(Vec3 vec) override
    {
        for (auto pair : _parts)
            pair.second->translate(vec);
        return *this;
    }

    Module3D& rotate(Vec3 vec) override
    {
        for (auto pair : _parts)
            pair.second->rotate(vec);
        return *this;
    }

    using Part3D::rotate;

    Module3D& mirror(Vec3 vec) override
    {
        for (auto pair : _parts)
            pair.second->mirror(vec);
        return *this;
    }

    using Part3D::mirror;

    void render3D(IndentWriter& writer) override
    {
        writer << _get_final_form();
    }

    template <typename T>
    requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& operator+=(T&& part)
    {
        return _append(std::forward<T>(part), _CompositionT::tAdd);
    }

    template <typename T>
    requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& operator-=(T&& part)
    {
        return _append(std::forward<T>(part), _CompositionT::tCut);
    }

    std::list<FlatPart> get_all_flats() const override;

  protected:
    Component _get_final_form() override;

    std::shared_ptr<Part3D> _clone() override
    {
        return std::make_shared<Module3D>(*this);
    }

  private:
    template <typename T1, typename T2>
    requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
          && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
    Module3D(T1&& part1, T2&& part2, _CompositionT ctype)
    {
        _append(std::forward<T1>(part1), _CompositionT::tAdd);
        _append(std::forward<T2>(part2), ctype);
    }

    template <typename T>
    requires std::derived_from<typename std::remove_reference<T>::type, Part3D>
    Module3D& _append(T&& part, _CompositionT ctype)
    {
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


    std::vector<std::pair<_CompositionT, std::shared_ptr<Part3D>>> _parts;

    Color _color = { -1, 0, 0 };
}; // class Module3D

template <typename T1, typename T2>
requires std::derived_from<typename std::remove_reference<T1>::type, Part3D>
      && std::derived_from<typename std::remove_reference<T2>::type, Part3D>
Module3D operator+(T1&& part1, T2&& part2)
{
    if (std::same_as<typename std::remove_reference<T1>::type, Module3D>) {
        Module3D mod { std::forward<T1>(part1) };
        return mod._append(
            std::forward<T2>(part2), Module3D::_CompositionT::tAdd
        );
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
Module3D operator-(T1&& part1, T2&& part2)
{
    return Module3D(
        std::forward<T1>(part1),
        std::forward<T2>(part2),
        Module3D::_CompositionT::tCut
    );
}

#endif // PARTS_HPP_INCLUDED
