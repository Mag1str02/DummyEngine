#include "DummyEngine/Core/Objects/LightSources/PointLight.h"

namespace DE {
PointLight::PointLight() {
}
PointLight::PointLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 position, Vec3 clq) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->position = position;
    this->clq = clq;
}
PointLight::~PointLight() {
}
}  // namespace DE