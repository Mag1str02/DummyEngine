#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE {
    std::string TextureFormatToStr(TextureFormat format) {
        switch (format) {
            case TextureFormat::RED: return "RED";
            case TextureFormat::RGB: return "RGB";
            case TextureFormat::RGBA: return "RGBA";
            case TextureFormat::None: return "None";
            case TextureFormat::Depth: return "Depth";
            default: return "Unknown";
        }
    }
    U32 PixelSize(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA: return 4;
            case TextureFormat::RGB: return 3;
            case TextureFormat::RED: return 1;
            default: return 0;
        }
    }

    std::string ShaderPartTypeToString(ShaderPartType type) {
        switch (type) {
            case ShaderPartType::Vertex: return "Vertex";
            case ShaderPartType::Fragment: return "Fragment";
            case ShaderPartType::Geometry: return "Geometry";
            default: return "None";
        }
    }
    ShaderPartType StringToShaderPartType(const std::string& type) {
        if (type == "Vertex") return ShaderPartType::Vertex;
        if (type == "Fragment") return ShaderPartType::Fragment;
        if (type == "Geometry") return ShaderPartType::Geometry;
        return ShaderPartType::None;
    }

    TextureData::TextureData() : m_Data(nullptr), m_Width(0), m_Height(0), m_Format(TextureFormat::None) {}

    TextureData::TextureData(const U8* data, U32 width, U32 height, TextureFormat format) : m_Data(nullptr) {
        SetData(data, width, height, format);
    }
    TextureData::~TextureData() {
        delete[] m_Data;
    }
    void TextureData::SetData(const U8* data, U32 width, U32 height, TextureFormat format) {
        if (data) {
            delete[] m_Data;
        }
        m_Width  = width;
        m_Height = height;
        m_Format = format;
        m_Data   = (U8*)malloc(width * height * PixelSize(format));
        std::memcpy(m_Data, data, width * height * PixelSize(format));
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