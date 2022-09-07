#pragma once
#include "Addition/Base.h"

namespace DE
{
    enum class TextureFormat
    {
        None = 0,
        RED,
        RGB,
        RGBA,
        DepthStencil
    };
    enum CubeSide
    {
        Right = 0,
        Left = 1,
        Top = 2,
        Bottom = 3,
        Back = 5,
        Front = 4,
    };
    uint32_t PixelSize(TextureFormat format);

    struct ShaderPart
    {
        ShaderPartType type;
        Path path;
    };

    std::string ShaderPartTypeToString(ShaderPartType type);
    ShaderPartType StringToShaderPartType(const std::string& type);

    class TextureData
    {
    public:
        TextureData();
        TextureData(const uint8_t* data, uint32_t width, uint32_t height, TextureFormat format);
        ~TextureData();

        void SetData(const uint8_t* data, uint32_t width, uint32_t height, TextureFormat format);

        uint32_t Width() const { return m_Width; }
        uint32_t Height() const { return m_Height; }
        TextureFormat Format() const { return m_Format; }
        uint32_t Channels() const { return PixelSize(m_Format); }
        uint8_t* Data() { return m_Data; }
        const uint8_t* Data() const { return m_Data; }

    private:
        uint8_t* m_Data;
        uint32_t m_Width;
        uint32_t m_Height;
        TextureFormat m_Format;
    };

    struct MaterialData
    {
        Vec3 ambient_color;
        Vec3 diffuse_color;
        Vec3 specular_color;
        float shininess;
        Ref<TextureData> specular_map;
        Ref<TextureData> diffuse_map;
    };

    struct RenderSubMeshData
    {
        std::vector<Vertex3D> vertices;
        std::vector<uint32_t> indices;
        MaterialData material;

        RenderSubMeshData& operator+=(const RenderSubMeshData& other);
    };
    struct RenderMeshData
    {
        std::vector<RenderSubMeshData> meshes;

        void Compress();
    };
}  // namespace DE