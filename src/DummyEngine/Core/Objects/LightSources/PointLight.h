#pragma once

#include "Addition/Base.h"

namespace DE {
class PointLight {

public:
    Vec3 ambient = Vec3(1.0f);
    Vec3 diffuse = Vec3(1.0f);
    Vec3 specular = Vec3(1.0f);
    Vec3 position = Vec3(0.0f);
    Vec3 clq = Vec3(1.0f, 0.007f, 0.0002f);

    PointLight();
    PointLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 clq);
    virtual ~PointLight();
};
}  // namespace DE