#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class Texture {
    public:
        enum class Channels { None = 0, R, RG, RGB, RGBA, Depth };
        enum class Format { None = 0, U8, U16, F16, F32 };

        static Format      DataFormat(TextureFormat format);
        static Channels    DataChannels(TextureChannels channels);
        static std::string FormatToStr(Format format);
        static std::string ChannelsToStr(Channels channel);

        virtual ~Texture() = default;

        virtual U32 Width() const      = 0;
        virtual U32 Height() const     = 0;
        virtual U32 RendererId() const = 0;

        // virtual void SetData(const void* data, U32 size) = 0;

        virtual void Resize(U32 width, U32 height) = 0;
        virtual void Bind(U32 slot) const          = 0;

        static Ref<Texture> Create(Texture::Channels channels = Texture::Channels::RGBA, Texture::Format format = Texture::Format::U8);
        static Ref<Texture> Create(U32               width,
                                   U32               height,
                                   Texture::Channels channels = Texture::Channels::RGBA,
                                   Texture::Format   format   = Texture::Format::U8);
        static Ref<Texture> Create(const TextureData& texture_data);
    };
}  // namespace DE