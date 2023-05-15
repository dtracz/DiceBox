#include <cmath>
#include <stdexcept>
#include "base.h"
#include "core.h"



Part3D& Part3D::rotate(Vec3 vec, Vec3 center) {
    translate(-center);
    rotate(vec);
    translate(center);
    return *this;
}


Part3D& Part3D::mirror(Vec3 vec, Vec3 center) {
    translate(-center);
    mirror(vec);
    translate(center);
    return *this;
}


FlatPart& FlatPart::translate(Vec3 vec) {
    if (!_transforms.empty() &&
         _transforms.back().first == _TransformT::tTranslate) {
        auto pair = _transforms.back();
        _transforms.pop_back();
        Vec3 prev_transform = pair.second;
        vec += prev_transform;
    }
    if (vec != Vec3::ZERO())
        _transforms.emplace_back(_TransformT::tTranslate, vec);
    return *this;
}


Component FlatPart::_get_final_form() {
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
                part.rotate(-transform_vec.x,
                            -transform_vec.y,
                            -transform_vec.z);
                break;
            case _TransformT::tMirror:
                part.mirror(transform_vec.x,
                            transform_vec.y,
                            transform_vec.z);
                break;
        }
    }
    if (_color.is_valid())
        part.color(_color.x, _color.y, _color.z, _color.alpha);
    return part;
}



Module3D::Module3D(Module3D& other) {
    for (auto& pair : other._parts) {
        auto type = pair.first;
        std::shared_ptr<Part3D> shp = pair.second;
        _parts.emplace_back(type, shp->_clone());
    }
}


Component Module3D::_get_final_form() {
    if (_parts.empty())
        throw std::runtime_error("this Module3D is empty");
    if (_parts.front().first == _CompositionT::tCut)
        throw std::runtime_error("first Part3D of the Module3D needs to be added");
    auto part = _parts.front().second->_get_final_form();
    for (size_t i = 0; i < _parts.size(); i++) {
        auto pair = _parts[i];
        _CompositionT ctype = pair.first;
        auto shp = pair.second;
        switch (ctype) {
            case _CompositionT::tAdd:
                part = part + shp->_get_final_form();
                break;
            case _CompositionT::tCut:
                part = part - shp->_get_final_form();
                break;
        }
    }
    if (_color.is_valid())
        part.color(_color.x, _color.y, _color.z, _color.alpha);
    return part;
}

