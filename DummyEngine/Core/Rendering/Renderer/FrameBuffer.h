#pragma once

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    struct FrameBufferProperties {
        U32 width  = 0;
        U32 height = 0;
    };

    struct FrameBufferAttchment {
        Ref<Texture>    m_Texture;
        TextureChannels m_Format;
    };

    class FrameBuffer {
    public:
        virtual void Bind()   = 0;
        virtual void UnBind() = 0;
        virtual void Rebuild() = 0;

        virtual void Resize(U32 width, U32 height) = 0;
        virtual U32  GetWidth() const              = 0;
        virtual U32  GetHeight() const             = 0;

        virtual void AddColorAttachment(TextureChannels format)              = 0;
        virtual void AddColorAttachment(Ref<CubeMap>, U32 side, U32 lod = 0) = 0;
        virtual void SetDepthAttachment(TextureChannels format)              = 0;
        virtual bool Valid() const                                           = 0;

        virtual Ref<Texture> GetColorAttachment(U32 attachment_id) = 0;
        virtual Ref<Texture> GetDepthAttachment()                  = 0;

        virtual const FrameBufferProperties& GetProperties() const = 0;
        static Ref<FrameBuffer>              Create(const FrameBufferProperties& properties);
    };
}  // namespace DE