#pragma once    

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/Texture.h"
#include "Core/Rendering/Renderer/VertexArray.h"

namespace DE
{
    struct Vertex3D
    {
        Vec3 position;
        Vec3 normal;
        Vec2 tex_coords;
    };

   
    struct MaterialData
    {
        Vec3 ambient_color;
        Vec3 diffuse_color;
        Vec3 specular_color;
        float shininess;
        TextureData specular_map;
        TextureData diffuse_map;
        TextureData normal_map;
    };

    struct RenderMeshData
    {
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
        MaterialData material;

        RenderMeshData& operator+=(const RenderMeshData& other);
    };
    struct RenderModelData
    {
        std::vector<RenderMeshData> meshes;

        void Compress();
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
        std::vector<RenderMesh> meshes;

        void FillData(const RenderModelData& data);
    };

}  // namespace DE