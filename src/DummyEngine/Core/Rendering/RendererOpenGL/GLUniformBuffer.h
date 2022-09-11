#pragma once

#include <GLAD/glad.h>

#include "Core/Rendering/Renderer/UniformBuffer.h"

namespace DE
{
    class GLUniformBuffer : public UniformBuffer
    {
    public:
        GLUniformBuffer(const BufferLayout& layout, uint32_t size);
        virtual ~GLUniformBuffer();

        GLUniformBuffer()                             = delete;
        GLUniformBuffer(const GLUniformBuffer& other) = delete;
        GLUniformBuffer(GLUniformBuffer&& other)      = delete;
        GLUniformBuffer& operator=(const GLUniformBuffer& other) = delete;
        GLUniformBuffer& operator=(GLUniformBuffer&& other) = delete;

        virtual void                Bind(uint32_t index) const;
        virtual LocalBufferNode     at(uint32_t index);
        virtual void                PushData() const;
        virtual const BufferLayout& GetLayout() const;

    private:
        GLuint       m_BufferId;
        LocalBuffer  m_LocalBuffer;
        BufferLayout m_Layout;
        uint32_t     m_Size;
    };
}  // namespace DE