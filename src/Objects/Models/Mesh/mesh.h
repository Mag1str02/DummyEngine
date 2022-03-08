#pragma once

#include <glm/glm.hpp>

#include "../../../Wrappings/ShaderProgram/shader_program.h"
#include "../../../Wrappings/Texture/texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords = glm::vec2(0.0f, 0.0f);
};

class Mesh {
public:
    Mesh();
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    void Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    void Draw(ShaderProgram& shader_program);

private:
    unsigned int VAO, VBO, EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    size_t indices_amount_;

    void BuildMesh();
};