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
        Ref<TextureData> specular_map;
        Ref<TextureData> diffuse_map;
        Ref<TextureData> normal_map;
    };

    struct RenderMeshData
    {
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
        MaterialData material;

        RenderMeshData& operator+=(const RenderMeshData& other);
    };
    struct LoadingProperties
    {
        bool flip_uvs;
        bool compress;
    };
    struct RenderModelData
    {
        std::vector<RenderMeshData> meshes;
        Path path;

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

        void FillData(RenderMeshData data);
    };
    class RenderModel
    {
    public:
        RenderModel() {}

        void FillData(Ref<RenderModelData> data, const std::string& name);
        void AddInstanceBuffer(Ref<VertexBuffer> instance_buffer);

        const std::string& GetName() const;

    private:
        friend class Renderer;
        
        std::vector<RenderMesh> m_Meshes;
        std::vector<Ref<VertexBuffer>> m_InstanceBuffers;

        std::string m_Name;
    };
    struct ModelOwner
    {
        RenderModel render_model;
        Ref<RenderModelData> data;
        LoadingProperties properties;
    };

}  // namespace DE