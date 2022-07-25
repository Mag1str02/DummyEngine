#include "Core/Objects/LightSources/DirectionalLight.h"

namespace DE {
DirectionalLight::DirectionalLight() {
}
DirectionalLight::DirectionalLight(Vec3 ambient, Vec3 diffuse, Vec3 specular, Vec3 direction) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
}
}  // namespace DE