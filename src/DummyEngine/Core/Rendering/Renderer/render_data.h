#pragma once

#include <GLAD/glad.h>

#include "DummyEngine/Addition/base.h"


namespace DE
{
    enum class TextureFormat
    {
        None = 0,
        RED,
        RGB,
        RGBA
    };
    struct Vertex3D
    {
        Vec3 position;
        Vec3 normal;
        Vec2 tex_coords;
    };

    struct TextureData
    {
        Ref<unsigned char*> data;
        uint32_t width;
        uint32_t height;
        TextureFormat format;
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

}  // namespace DE