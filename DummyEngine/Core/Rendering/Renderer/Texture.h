#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Core/ResourceManaging/Resources/Texture.hpp"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class FrameBuffer;
    class Texture {
    public:
        enum class Channels { None = 0, R, RG, RGB, RGBA, Depth };
        enum class Format { None = 0, U8, U16, F16, F32 };

        static Format      DataFormat(TextureFormat format);
        static Channels    DataChannels(TextureChannels channels);
        static std::string FormatToStr(Format format);
        static std::string ChannelsToStr(Channels channel);

        virtual ~Texture() = default;

        virtual U32      GetWidth() const      = 0;
        virtual U32      GetHeight() const     = 0;
        virtual U32      GetRendererId() const = 0;
        virtual Format   GetFormat() const     = 0;
        virtual Channels GetChannels() const   = 0;

        virtual void Copy(Ref<FrameBuffer> buffer, U32 attachment_id) = 0;
        virtual void SetFormat(Format format)                         = 0;
        virtual void SetChannels(Channels channels)                   = 0;
        virtual void Resize(U32 width, U32 height)                    = 0;
        virtual void Bind(U32 slot) const                             = 0;
        virtual void Remove()                                         = 0;

        static Ref<Texture> Create(Channels channels = Channels::RGBA, Format format = Format::U8);
        static Ref<Texture> Create(U32 width, U32 height, Channels channels = Channels::RGBA, Format format = Format::U8);
        static Ref<Texture> Create(const TextureResource& texture_data);
    };
}  // namespace DE