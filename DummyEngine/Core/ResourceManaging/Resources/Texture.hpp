#pragma once

#include "DummyEngine/Core/ResourceManaging/Resource.hpp"
#include "Texture.hpp"
namespace DE {

    class Texture;
    enum class TextureChannels { None = 0, RED, RG, RGB, RGBA };
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
    U32 ChannelAmount(TextureChannels format);
    U32 FormatSize(TextureFormat format);

    class TextureResource : public Resource, public std::enable_shared_from_this<TextureResource> {
    public:
        TextureResource();
        TextureResource(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format = TextureFormat::U8);
        ~TextureResource();
        void SetData(const void* data, U32 width, U32 height, TextureChannels channels, TextureFormat format = TextureFormat::U8);

        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

        U32             PixelSize() const;
        U32             Width() const { return m_Width; }
        U32             Height() const { return m_Height; }
        TextureChannels Channels() const { return m_Channels; }
        TextureFormat   Format() const { return m_Format; }
        void*       Data() { return m_Data; }
        const void* Data() const { return m_Data; }

    private:
        // GPU
        Ref<Texture>  m_texture;
        // RAM
        void*           m_Data{};
        U32             m_Width{};
        U32             m_Height{};
        TextureChannels m_Channels;
        // Loading params
        Path          m_Path;
        bool          m_Flip_UVs = true;
        TextureFormat m_Format   = TextureFormat::U8;
    };

}