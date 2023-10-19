#include "DummyEngine/Core/ResourceManaging/RawData.h"

#include "Resource.hpp"

namespace DE {
    std::string MaterialTypeToStr(MaterialType type) {
        switch (type) {
            case MaterialType::PBR: return "PBR";
            case MaterialType::Phong: return "Phong";
            case MaterialType::None: return "None";
            default: DE_ASSERT(false, "Unsupported material type");
        }
        return "None";
    }
    MaterialType MaterialTypeFromStr(const std::string& str) {
        if (str == "PBR") return MaterialType::PBR;
        if (str == "Phong") return MaterialType::Phong;
        return MaterialType::None;
    }
    U32 ChannelAmount(TextureChannels format) {
        switch (format) {
            case TextureChannels::RGBA: return 4;
            case TextureChannels::RGB: return 3;
            case TextureChannels::RG: return 2;
            case TextureChannels::RED: return 1;
            case TextureChannels::None: return 0;
            default: DE_ASSERT(false, "Unsupported texture format"); break;
        }
        return 0;
    }
    U32 FormatSize(TextureFormat format) {
        switch (format) {
            case TextureFormat::Float: return sizeof(float);
            case TextureFormat::U8: return sizeof(U8);
            case TextureFormat::None: return 0;
            default: DE_ASSERT(false, "Unsupported texture format"); break;
        }
        return 0;
    }

    std::string ShaderPartTypeToString(ShaderPartType type) {
        switch (type) {
            case ShaderPartType::Vertex: return "Vertex";
            case ShaderPartType::Fragment: return "Fragment";
            case ShaderPartType::Geometry: return "Geometry";
            case ShaderPartType::None: return "None";
            default: DE_ASSERT(false, "Unsupported shader part type"); break;
        }
        return "None";
    }
    ShaderPartType StringToShaderPartType(const std::string& type) {
        if (type == "Vertex") return ShaderPartType::Vertex;
        if (type == "Fragment") return ShaderPartType::Fragment;
        if (type == "Geometry") return ShaderPartType::Geometry;
        return ShaderPartType::None;
    }

    TextureData::TextureData() : m_Data(nullptr), m_Width(0), m_Height(0), m_Channels(TextureChannels::None), m_Format(TextureFormat::None) {}

    TextureData::TextureData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) : m_Data(nullptr) {
        SetData(data, width, height, channels, format);
    }
    TextureData::~TextureData() {
        free(m_Data);
    }
    void TextureData::SetData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) {
        if (m_Data) {
            free(m_Data);
        }
        size_t size = width * height * ChannelAmount(channels) * FormatSize(format);
        DE_ASSERT(size > 0, "Wrong texture data size");
        m_Data = malloc(size);
        std::memcpy(m_Data, data, size);
        m_Width    = width;
        m_Height   = height;
        m_Channels = channels;
        m_Format   = format;
    }
    U32 TextureData::PixelSize() const {
        return ChannelAmount(m_Channels) * FormatSize(m_Format);
    }

    RenderSubMeshData& RenderSubMeshData::operator+=(const RenderSubMeshData& other) {
        size_t sz = vertices.size();
        for (size_t i = 0; i < other.vertices.size(); ++i) {
            vertices.push_back(other.vertices[i]);
        }
        for (size_t i = 0; i < other.indices.size(); ++i) {
            indices.push_back(other.indices[i] + sz);
        }
        return *this;
    }

    void RenderMeshData::Compress() {
        std::vector<RenderSubMeshData> new_vec(1);
        new_vec.back() = meshes.front();
        for (size_t i = 1; i < meshes.size(); ++i) {
            new_vec.back() += meshes[i];
        }
        meshes = new_vec;
    }

}  // namespace DE