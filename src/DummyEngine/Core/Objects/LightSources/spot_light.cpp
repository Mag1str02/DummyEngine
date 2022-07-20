#include "DummyEngine/Core/Objects/LightSources/spot_light.h"

namespace DE {
SpotLight::SpotLight() {
}
SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 clq, glm::vec3 direction, float outer_cone_cos, float inner_cone_cos)
    : PointLight(ambient, diffuse, specular, position, clq) {
    this->direction = direction;
    this->outer_cone_cos = outer_cone_cos;
    this->inner_cone_cos = inner_cone_cos;
}
SpotLight::~SpotLight() {
}
}  // namespace DE