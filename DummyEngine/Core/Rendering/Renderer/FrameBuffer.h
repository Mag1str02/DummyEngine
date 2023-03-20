#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    struct FrameBufferProperties {
        uint32_t width  = 0;
        uint32_t height = 0;
    };

    struct FrameBufferAttchment {
        Ref<Texture>  m_Texture;
        TextureFormat m_Format;
    };

    class FrameBuffer {
    public:
        virtual void Bind()   = 0;
        virtual void UnBind() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual void AddColorAttachment(TextureFormat format) = 0;
        virtual void SetDepthAttachment(TextureFormat format) = 0;

        virtual Ref<Texture> GetColorAttachment(uint32_t attachment_id) = 0;
        virtual Ref<Texture> GetDepthAttachment()                       = 0;

        virtual const FrameBufferProperties& GetProperties() const = 0;
        static Ref<FrameBuffer>              Create(const FrameBufferProperties& properties);
    };
}  // namespace DE