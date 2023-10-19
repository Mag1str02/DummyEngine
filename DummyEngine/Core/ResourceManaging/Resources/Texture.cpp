#include "Texture.hpp"

#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"

namespace DE {

    void DE::TextureResource::Prepare() {
        TextureLoader::Load(shared_from_this(), m_Path, m_Flip_UVs, m_Format);
        Resource::Prepare();
    }
    void DE::TextureResource::Load() {
        m_texture = Texture::Create(*this); // Renderer texture
        Resource::Load();
    }
    void DE::TextureResource::Unload() {
        m_texture->Remove();
        m_texture = nullptr; // TODO: ideally this should free Texture obj, tbc
        Resource::Unload();
    }
    void DE::TextureResource::Remove() {
        free(m_Data);
        Resource::Remove();
    }
    U32 TextureResource::PixelSize() const {
        return ChannelAmount(m_Channels) * FormatSize(m_Format);
    }
    void TextureResource::SetData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) {
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
    TextureResource::TextureResource(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format) {
        SetData(data, width, height, channels, format);
    }
    TextureResource::TextureResource() : m_Data(nullptr), m_Width(0), m_Height(0), m_Channels(TextureChannels::None), m_Format(TextureFormat::None) {}
    TextureResource::~TextureResource() {
        if (GetState() == LOADED_TO_GPU) {
            Unload();
        }
        if (GetState() == LOADED_TO_RAM) {
            Remove();
        }
        DE_ASSERT(GetState() == NOT_LOADED, "Texture ", GetID(), "did not unload");
    }

}  // namespace DE