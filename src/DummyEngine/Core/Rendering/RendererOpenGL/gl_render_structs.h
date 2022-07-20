#pragma once

#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_ebo.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_vao.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_vbo.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_texture_2D.h"
#include "DummyEngine/Core/Rendering/Renderer/render_data.h"

namespace DE {

struct GLMaterial {
    glm::vec3 ambient_color;
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float shininess;
    GLTexture2D specular_map;
    GLTexture2D diffuse_map;
    GLTexture2D normal_map;
};

struct GLRenderMesh {
    GLMaterial material;
    GLVertexArray vertex_array;
    GLVertexBuffer vertex_buffer;
    GLElementBuffer element_buffer;
    size_t indices_amount;

    void FillData(const RenderMeshData& data);
};
struct RenderModel {
    std::vector<GLRenderMesh> render_meshes;

    void FillData(const RenderModelData& data);
};
}  // namespace DE