#include <cmath>
#include <stdexcept>
#include "base.h"
#include "core.h"


const double EPS = 1e-8;


bool Vec3::operator==(Vec3 other) const {
    return std::abs(x - other.x) < EPS
        && std::abs(y - other.y) < EPS
        && std::abs(z - other.z) < EPS;
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


FlatPart& FlatPart::rotate(Vec3 vec, Vec3 center) {
    translate(-center);
    rotate(vec);
    translate(center);
    return *this;
}


void FlatPart::render3D(IndentWriter& writer) {
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

