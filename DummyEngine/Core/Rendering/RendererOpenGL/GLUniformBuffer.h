#pragma once

#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"

#include <glad/glad.h>

namespace DummyEngine {
    class GLUniformBuffer : public UniformBuffer {
    public:
        GLUniformBuffer(const BufferLayout& layout, U32 size);
        virtual ~GLUniformBuffer();

        GLUniformBuffer()                                        = delete;
        GLUniformBuffer(const GLUniformBuffer& other)            = delete;
        GLUniformBuffer(GLUniformBuffer&& other)                 = delete;
        GLUniformBuffer& operator=(const GLUniformBuffer& other) = delete;
        GLUniformBuffer& operator=(GLUniformBuffer&& other)      = delete;

        virtual void                Bind(U32 index) const override;
        virtual LocalBufferNode     At(U32 index) override;
        virtual void                PushData() const override;
        virtual const BufferLayout& GetLayout() const override;

    private:
        GLuint       buffer_id_;
        LocalBuffer  local_buffer_;
        BufferLayout layout_;
        U32          size_;
    };
}  // namespace DummyEngine