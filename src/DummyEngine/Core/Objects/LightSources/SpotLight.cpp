#include "DummyEngine/Core/Objects/LightSources/SpotLight.h"

namespace DE {
SpotLight::SpotLight() {
}
SpotLight::SpotLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 clq, Vec3 direction, float outer_cone_cos, float inner_cone_cos)
    : PointLight(ambient, diffuse, specular, position, clq) {
    this->direction = direction;
    this->outer_cone_cos = outer_cone_cos;
    this->inner_cone_cos = inner_cone_cos;
}
SpotLight::~SpotLight() {
}
}  // namespace DE