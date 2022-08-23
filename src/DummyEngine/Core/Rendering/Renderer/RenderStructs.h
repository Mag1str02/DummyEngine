#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/Texture.h"
#include "Core/Rendering/Renderer/VertexArray.h"

namespace DE
{
    struct Material
    {
        Vec3 ambient_color;
        Vec3 diffuse_color;
        Vec3 specular_color;
        float shininess;
        Ref<Texture> specular_map;
        Ref<Texture> diffuse_map;
    };

    struct RenderSubMesh
    {
        Material material;
        Ref<VertexArray> vertex_array;

        void FillData(const RenderSubMeshData& data);
    };
    struct RenderMesh
    {
        std::vector<RenderSubMesh> meshes;

        RenderMesh(){}
        RenderMesh(Ref<RenderMeshData> data);
        void FillData(Ref<RenderMeshData> data);
    };

}  // namespace DE