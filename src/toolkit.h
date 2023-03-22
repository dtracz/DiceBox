#ifndef TOOLKIT_H_INCLUDED
#define TOOLKIT_H_INCLUDED

#include <stdexcept>
#include <vector>
#include <utility>
#include "core/Component2D.h"
#include "base.h"



template <typename T, class F>
inline T fold(const std::vector<T>& vec, F f) {
    if (vec.empty())
        throw std::runtime_error("cannot fold empty vector");
    T res = vec.front();
    for (size_t i = 1; i < vec.size(); i++)
        res = f(res, vec[i]);
    return res;
}


Component2D crenels(std::pair<double, double> crenel_dims,
                    double length, double offset=0);


Component2D crenellated_wall(std::pair<double, double> dims,
                             Vec3 top_crenel_params = Vec3::ZERO(),
                             Vec3 bottom_crenel_params = Vec3::ZERO(),
                             Vec3 left_crenel_params = Vec3::ZERO(),
                             Vec3 right_crenel_params = Vec3::ZERO());



#endif  // TOOLKIT_H_INCLUDED
