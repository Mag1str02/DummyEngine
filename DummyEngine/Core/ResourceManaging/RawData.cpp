#include "RawData.h"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

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

    TextureData::TextureData() : data_(nullptr), width_(0), height_(0), channels_(TextureChannels::None), format_(TextureFormat::None) {}

    TextureData::TextureData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) : data_(nullptr) {
        SetData(data, width, height, channels, format);
    }
    TextureData::~TextureData() {
        free(data_);
    }
    void TextureData::SetData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) {
        if (data_ != nullptr) {
            free(data_);
        }
        size_t size = width * height * ChannelAmount(channels) * FormatSize(format);
        DE_ASSERT(size > 0, "Wrong texture data size");
        data_ = malloc(size);
        std::memcpy(data_, data, size);
        width_    = width;
        height_   = height;
        channels_ = channels;
        format_   = format;
    }
    U32 TextureData::PixelSize() const {
        return ChannelAmount(channels_) * FormatSize(format_);
    }

    RenderSubMeshData& RenderSubMeshData::operator+=(const RenderSubMeshData& other) {
        size_t sz = Vertices.size();
        for (size_t i = 0; i < other.Vertices.size(); ++i) {
            Vertices.push_back(other.Vertices[i]);
        }
        for (size_t i = 0; i < other.Indices.size(); ++i) {
            Indices.push_back(other.Indices[i] + sz);
        }
        return *this;
    }

    void RenderMeshData::Compress() {
        std::vector<RenderSubMeshData> new_vec(1);
        new_vec.back() = Meshes.front();
        for (size_t i = 1; i < Meshes.size(); ++i) {
            new_vec.back() += Meshes[i];
        }
        Meshes = new_vec;
    }

}  // namespace DummyEngine