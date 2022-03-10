#pragma once

#include <glm/glm.hpp>

#include "../../../Wrappings/Texture/texture.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal = glm::vec3(-1.0);
    glm::vec2 tex_coords;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh &operator+=(const Mesh &other);
};