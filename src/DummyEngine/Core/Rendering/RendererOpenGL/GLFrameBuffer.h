#pragma once

#include <GLAD/glad.h>

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/FrameBuffer.h"

namespace DE
{
    class GLFrameBuffer : public FrameBuffer
    {
    public:
        GLFrameBuffer(const FrameBufferProperties& properties);
        virtual ~GLFrameBuffer();

        GLFrameBuffer() = delete;
        GLFrameBuffer(const FrameBuffer& other) = delete;
        GLFrameBuffer(FrameBuffer&& other) = delete;
        GLFrameBuffer& operator=(const FrameBuffer& other) = delete;
        GLFrameBuffer& operator=(FrameBuffer&& other) = delete;

        virtual void Bind() override;
        virtual void UnBind() override;

        virtual void AddColorAttachment(TextureFormat format) override;
        virtual void SetDepthAttachment(TextureFormat format) override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual Ref<Texture> GetColorAttachment(uint32_t attachment_id) override;
        virtual Ref<Texture> GetDepthAttachment() override;

        virtual const FrameBufferProperties& GetProperties() const override;

    private:
        void Rebuild();

        GLuint m_BufferId;
        FrameBufferProperties m_Properties;

        FrameBufferAttchment m_DepthAttachment;
        std::vector<FrameBufferAttchment> m_ColorAttachments;
    };
}  // namespace DE