#pragma once
#include "DummyEngine/Utils/Base.h"

namespace DE {
    enum class MaterialType {
        None = 0,
        Phong,
        PBR,
    };
    enum class TextureChannels { None = 0, RED, RG, RGB, RGBA, Depth };
    enum class TextureFormat {
        None = 0,
        U8,
        Float,
    };

    enum CubeSide {
        Right  = 0,
        Left   = 1,
        Top    = 2,
        Bottom = 3,
        Back   = 5,
        Front  = 4,
    };
    std::string TextureFormatToStr(TextureChannels format);
    U32         ChannelAmount(TextureChannels format);
    U32         FormatSize(TextureFormat format);

    struct ShaderPart {
        ShaderPartType type;
        Path           path;
    };

    std::string    ShaderPartTypeToString(ShaderPartType type);
    ShaderPartType StringToShaderPartType(const std::string& type);

    class TextureData {
    public:
        TextureData();
        TextureData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format = TextureFormat::U8);
        ~TextureData();
        void SetData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format = TextureFormat::U8);

        U32             PixelSize() const;
        U32             Width() const { return m_Width; }
        U32             Height() const { return m_Height; }
        TextureChannels Channels() const { return m_Channels; }
        TextureFormat   Format() const { return m_Format; }
        // U32             Channels() const { return PixelSize(m_Format); }
        void*       Data() { return m_Data; }
        const void* Data() const { return m_Data; }

    private:
        void*           m_Data;
        U32             m_Width;
        U32             m_Height;
        TextureChannels m_Channels;
        TextureFormat   m_Format;
    };

    struct MaterialData {
        MaterialType type = MaterialType::PBR;
        float        shininess;

        Vec3 albedo   = Vec3(1.0f);
        Vec3 diffuse  = Vec3(1.0f);
        Vec3 specular = Vec3(1.0f);
        Vec3 orm      = Vec3(1.0f);
        Vec3 ambient  = Vec3(1.0f);

        Ref<TextureData> albedo_map;
        Ref<TextureData> diffuse_map;
        Ref<TextureData> specular_map;
        Ref<TextureData> normal_map;
        Ref<TextureData> orm_map;
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