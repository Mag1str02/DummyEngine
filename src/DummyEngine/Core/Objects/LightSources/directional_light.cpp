#include "DummyEngine/Core/Objects/LightSources/directional_light.h"

namespace DE {
DirectionalLight::DirectionalLight() {
}
DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
}
}  // namespace DE