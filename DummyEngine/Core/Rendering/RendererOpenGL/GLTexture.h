#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"

namespace DummyEngine {

    class GLTexture : public Texture {
    public:
        GLTexture()                                  = delete;
        GLTexture(const GLTexture& other)            = delete;
        GLTexture(GLTexture&& other)                 = delete;
        GLTexture& operator=(const GLTexture& other) = delete;
        GLTexture& operator=(GLTexture&& other)      = delete;

        GLTexture(U32 width, U32 height, Channels channels, Format format, bool depth_buffer);
        GLTexture(Texture::Channels channels, Format format);
        explicit GLTexture(const TextureData& data);

        virtual ~GLTexture();

        virtual U32      GetWidth() const override;
        virtual U32      GetHeight() const override;
        virtual U32      GetRendererId() const override;
        virtual Format   GetFormat() const override;
        virtual Channels GetChannels() const override;

        virtual void Copy(Ref<FrameBuffer> buffer, U32 attachment_id) override;
        virtual void SetFormat(Format format) override;
        virtual void SetChannels(Channels channels) override;
        virtual void Resize(U32 width, U32 height) override;
        virtual void Bind(U32 slot) const override;

    private:
        void Invalidate(GLenum data_type = GL_UNSIGNED_BYTE, const void* data = nullptr);

        friend class GLFrameBuffer;

        GLuint   texture_id_;
        U32      width_;
        U32      height_;
        Format   format_;
        Channels channels_;
    };

}  // namespace DummyEngine