#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include "base/geometry.h"
#include "core/Component2D.h"


Component2D crenels(Vec2 crenel_dims,
                    double length, double offset=0);


Component2D crenellated_wall(Vec2 dims,
                             Vec3 top_crenel_params = Vec3::ZERO(),
                             Vec3 bottom_crenel_params = Vec3::ZERO(),
                             Vec3 left_crenel_params = Vec3::ZERO(),
                             Vec3 right_crenel_params = Vec3::ZERO());


#endif  // STRUCTURES_H_INCLUDED
