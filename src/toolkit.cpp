#include <utility>
#include <vector>
#include "core/Component2D.h"
#include "components/Square.h"
#include "core/Union.h"
#include "toolkit.h"
#include "base.h"



Component2D crenels(std::pair<double, double> crenel_dims,
                    double length, double offset) {
    double x = crenel_dims.first;
    double y = crenel_dims.second;
    std::vector<Component2D> crenels_v;
    while (offset < length) {
        double real_x = offset + x < length
                      ? x + std::min(offset, .0)
                      : length - offset;
        if (real_x <= 0)
            continue;
        crenels_v.push_back(
            Square::create(real_x, y, false).translate(std::max(offset, .0), 0, 0)
        );
        offset += 2*x;
    }
    return fold(crenels_v, [](auto c1, auto c2) { return c1 + c2; });
}


Component2D crenellated_wall(std::pair<double, double> dims,
                             Vec3 top_crenel_params,
                             Vec3 bottom_crenel_params,
                             Vec3 left_crenel_params,
                             Vec3 right_crenel_params) {
    double x = dims.first - left_crenel_params.y - right_crenel_params.y;
    double y = dims.second - top_crenel_params.y - bottom_crenel_params.y;
    auto wall = Square::create(x, y, false);
    wall.translate(left_crenel_params.y, bottom_crenel_params.y, 0);
    if (top_crenel_params.y > 0) {
        auto top_crenels = crenels(
                {top_crenel_params.x, top_crenel_params.y},
                x, top_crenel_params.z
        );
        top_crenels.translate(left_crenel_params.y, dims.second - top_crenel_params.y, 0);
        wall = wall + top_crenels;
    }
    if (bottom_crenel_params.y > 0) {
        auto bottom_crenels = crenels(
                {bottom_crenel_params.x, bottom_crenel_params.y},
                x, bottom_crenel_params.z
        );
        bottom_crenels.translate(left_crenel_params.y, 0, 0);
        wall = wall + bottom_crenels;
    }
    if (left_crenel_params.y > 0) {
        auto left_crenels = crenels(
                {left_crenel_params.x, left_crenel_params.y},
                dims.second, left_crenel_params.z
        );
        left_crenels.rotate(0, 0, 90);
        left_crenels.translate(left_crenel_params.y, 0, 0);
        wall = wall + left_crenels;
    }
    if (right_crenel_params.y > 0) {
        auto right_crenels = crenels(
                {right_crenel_params.x, right_crenel_params.y},
                dims.second, right_crenel_params.z);
        right_crenels.rotate(0, 0, -90);
        right_crenels.translate(dims.first - right_crenel_params.y, dims.second, 0);
        wall = wall + right_crenels;
    }
    return wall;
}

