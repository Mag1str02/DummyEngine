#pragma once

#include "DummyEngine/Core/Animations/Animation.h"

#include <string>

namespace DummyEngine {

    enum class MaterialType {
        None = 0,
        Phong,
        PBR,
    };
    enum class TextureChannels {
        None = 0,
        RED,
        RG,
        RGB,
        RGBA,
    };
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

    std::string  MaterialTypeToStr(MaterialType type);
    MaterialType MaterialTypeFromStr(const std::string& str);
    U32          ChannelAmount(TextureChannels format);
    U32          FormatSize(TextureFormat format);

    struct ShaderPart {
        ShaderPartType Type;
        Path           Path;
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
        U32             Width() const { return width_; }
        U32             Height() const { return height_; }
        TextureChannels Channels() const { return channels_; }
        TextureFormat   Format() const { return format_; }
        void*           Data() { return data_; }
        const void*     Data() const { return data_; }

    private:
        void*           data_;
        U32             width_;
        U32             height_;
        TextureChannels channels_;
        TextureFormat   format_;
    };

    struct MaterialData {
        MaterialType Type = MaterialType::PBR;
        float        Shininess;

        Vec3 Albedo   = Vec3(1.0f);
        Vec3 Diffuse  = Vec3(1.0f);
        Vec3 Specular = Vec3(1.0f);
        Vec3 ORM      = Vec3(1.0f);
        Vec3 Ambient  = Vec3(1.0f);
        Vec3 Emission = Vec3(1.0f);

        Ref<TextureData> AlbedoMap;
        Ref<TextureData> DiffuseMap;
        Ref<TextureData> SpecularMap;
        Ref<TextureData> NormalMap;
        Ref<TextureData> ORMMap;
        Ref<TextureData> EmissionMap;
    };

    struct RenderSubMeshData {
        std::vector<Vertex3D> Vertices;
        std::vector<U32>      Indices;
        MaterialData          Material;

        RenderSubMeshData& operator+=(const RenderSubMeshData& other);
    };
    struct RenderMeshData {
        Ref<Animation>                 Animation;
        std::vector<RenderSubMeshData> Meshes;

        void Compress();
    };
}  // namespace DummyEngine