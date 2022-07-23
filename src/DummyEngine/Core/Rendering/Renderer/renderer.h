#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/vertex_array.h"
#include "DummyEngine/Core/Rendering/Renderer/texture.h"

namespace DE
{
    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };

    struct Material
    {
        Vec3 ambient_color;
        Vec3 diffuse_color;
        Vec3 specular_color;
        float shininess;
        Ref<Texture> specular_map;
        Ref<Texture> diffuse_map;
        Ref<Texture> normal_map;
    };
    struct RenderMesh
    {
        Material material;
        Ref<VertexArray> vertex_array;

        void FillData(const RenderMeshData& data);
    };
    struct RenderModel
    {
        std::vector<RenderMesh> render_meshes;

        void FillData(const RenderModelData& data);
    };

    class Renderer
    {
    public:
        static void Init(API api);
        static void Terminate();
        static API CurrentAPI();

    private:
        inline static API _render_api;
    };

}  // namespace DE
