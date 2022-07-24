#pragma once

#include "DummyEngine/Core/Objects/LightSources/PointLight.h"
#include "DummyEngine/Addition/Base.h"

namespace DE {
class SpotLight : public PointLight {
public:
    Vec3 direction;
    float outer_cone_cos;
    float inner_cone_cos;

    SpotLight();
    SpotLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 clq, Vec3 direction, float outer_cone_cos, float inner_cone_cos);
    virtual ~SpotLight();
};
}  // namespace DE