#pragma once

#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"

#include <glad/glad.h>

namespace DummyEngine {

    class GLFrameBuffer : public FrameBuffer {
    public:
        explicit GLFrameBuffer(const FrameBufferProperties& properties);
        virtual ~GLFrameBuffer();

        GLFrameBuffer()                                    = delete;
        GLFrameBuffer(const FrameBuffer& other)            = delete;
        GLFrameBuffer(FrameBuffer&& other)                 = delete;
        GLFrameBuffer& operator=(const FrameBuffer& other) = delete;
        GLFrameBuffer& operator=(FrameBuffer&& other)      = delete;

        virtual void Bind() override;
        virtual void UnBind() override;
        virtual void Rebuild() override;

        virtual void AddColorAttachment(Texture::Format format, Texture::Channels channels) override;
        virtual void SetDepthAttachment(Texture::Format format) override;
        virtual void SetColorAttachment(Ref<Texture> texture, U32 id) override;
        virtual void AddColorAttachment(Ref<CubeMap> map, U32 side, U32 lod = 0) override;
        virtual void SetDepthAttachment(Ref<CubeMap> map) override;
        virtual bool Valid() const override;

        virtual void Resize(U32 width, U32 height) override;
        virtual U32  GetWidth() const override;
        virtual U32  GetHeight() const override;

        virtual Ref<Texture> GetColorAttachment(U32 attachment_id) override;
        virtual Ref<Texture> GetDepthAttachment() override;

        virtual const FrameBufferProperties& GetProperties() const override;

    private:
        GLuint                buffer_id_;
        FrameBufferProperties properties_;

        FrameBufferAttchment              depth_attachment_;
        std::vector<FrameBufferAttchment> color_attachments_;
    };

}  // namespace DummyEngine