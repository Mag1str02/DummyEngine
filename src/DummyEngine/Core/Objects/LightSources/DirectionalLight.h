#pragma once

#include "Addition/Base.h"

namespace DE {
class DirectionalLight {
public:
    Vec3 ambient = Vec3(1.0f);
    Vec3 diffuse = Vec3(1.0f);
    Vec3 specular = Vec3(1.0f);
    Vec3 direction = Vec3(0.0f, -1.0f, 0.0f);

    DirectionalLight();
    DirectionalLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 direction);
};
}  // namespace DE