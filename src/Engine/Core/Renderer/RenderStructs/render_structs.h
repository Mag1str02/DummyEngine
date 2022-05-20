#pragma once

#include "../Buffers/EBO/ebo.h"
#include "../Buffers/VAO/vao.h"
#include "../Buffers/VBO/vbo.h"
#include "../Textures/Texture2D/texture_2D.h"
#include "../RenderData/render_data.h"

namespace DE {

struct Material {
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float shininess;
    Texture2D specular_map;
    Texture2D diffuse_map;
    Texture2D normal_map;
};

struct RenderMesh {
    Material material;
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    ElementBuffer element_buffer;
    size_t indices_amount;

    void FillData(const RenderMeshData& data);
};
struct RenderModel {
    std::vector<RenderMesh> render_meshes;

    void FillData(const RenderModelData& data);
};
}  // namespace DE