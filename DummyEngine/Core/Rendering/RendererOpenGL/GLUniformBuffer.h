#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"

namespace DE {
    class GLUniformBuffer : public UniformBuffer {
    public:
        GLUniformBuffer(const BufferLayout& layout, U32 size);
        virtual ~GLUniformBuffer();

        GLUniformBuffer()                                        = delete;
        GLUniformBuffer(const GLUniformBuffer& other)            = delete;
        GLUniformBuffer(GLUniformBuffer&& other)                 = delete;
        GLUniformBuffer& operator=(const GLUniformBuffer& other) = delete;
        GLUniformBuffer& operator=(GLUniformBuffer&& other)      = delete;

        virtual void                Bind(U32 index) const;
        virtual LocalBufferNode     at(U32 index);
        virtual void                PushData() const;
        virtual const BufferLayout& GetLayout() const;

    private:
        GLuint       m_BufferId;
        LocalBuffer  m_LocalBuffer;
        BufferLayout m_Layout;
        U32          m_Size;
    };
}  // namespace DE