#pragma once

#include <glm/glm.hpp>

class PointLight {

public:
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 clq = glm::vec3(1.0f, 0.007f, 0.0002f);

    PointLight();
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 clq);
    virtual ~PointLight();
};