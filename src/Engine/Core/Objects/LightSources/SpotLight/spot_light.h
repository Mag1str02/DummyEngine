#pragma once

#include "../PointLight/point_light.h"
namespace DE {
class SpotLight : public PointLight {
public:
    glm::vec3 direction;
    float outer_cone_cos;
    float inner_cone_cos;

    SpotLight();
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 clq, glm::vec3 direction, float outer_cone_cos,
              float inner_cone_cos);
    virtual ~SpotLight();
};
}  // namespace DE