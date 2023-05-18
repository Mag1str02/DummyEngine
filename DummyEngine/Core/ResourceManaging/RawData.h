#pragma once
#include "DummyEngine/Utils/Base.h"

namespace DE {
    enum class TextureFormat { None = 0, RED, RA, RGB, RGBA, Depth };
    enum CubeSide {
        Right  = 0,
        Left   = 1,
        Top    = 2,
        Bottom = 3,
        Back   = 5,
        Front  = 4,
    };
    std::string TextureFormatToStr(TextureFormat format);
    U32         PixelSize(TextureFormat format);

    struct ShaderPart {
        ShaderPartType type;
        Path           path;
    };

    std::string    ShaderPartTypeToString(ShaderPartType type);
    ShaderPartType StringToShaderPartType(const std::string& type);

    class TextureData {
    public:
        TextureData();
        TextureData(const U8* data, U32 width, U32 height, TextureFormat format);
        ~TextureData();

        void SetData(const U8* data, U32 width, U32 height, TextureFormat format);

        U32           Width() const { return m_Width; }
        U32           Height() const { return m_Height; }
        TextureFormat Format() const { return m_Format; }
        U32           Channels() const { return PixelSize(m_Format); }
        U8*           Data() { return m_Data; }
        const U8*     Data() const { return m_Data; }

    private:
        U8*           m_Data;
        U32           m_Width;
        U32           m_Height;
        TextureFormat m_Format;
    };

    struct MaterialData {
        Vec3             ambient_color;
        Vec3             diffuse_color;
        Vec3             specular_color;
        float            shininess;
        Ref<TextureData> specular_map;
        Ref<TextureData> diffuse_map;
        Ref<TextureData> normal_map;
    };

    struct RenderSubMeshData {
        std::vector<Vertex3D> vertices;
        std::vector<U32>      indices;
        MaterialData          material;

        RenderSubMeshData& operator+=(const RenderSubMeshData& other);
    };
    struct RenderMeshData {
        std::vector<RenderSubMeshData> meshes;

        void Compress();
    };
}  // namespace DE