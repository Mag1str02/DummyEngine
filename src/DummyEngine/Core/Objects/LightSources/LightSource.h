#pragma once

#include "Addition/Base.h"

namespace DE
{
    enum class LightSourceType
    {
        None = 0,
        Point,
        Spot,
        Direction
    };

    struct LightSource
    {
        LightSourceType type;
        Vec3 ambient = Vec3(1.0f);
        Vec3 diffuse = Vec3(1.0f);
        Vec3 specular = Vec3(1.0f);
        Vec3 direction = Vec3(0.0f, -1.0f, 0.0f);
        Vec3 position = Vec3(0.0f);
        Vec3 clq = Vec3(1.0f, 0.007f, 0.0002f);
        float outer_cone_cos;
        float inner_cone_cos;
    };

}  // namespace DE