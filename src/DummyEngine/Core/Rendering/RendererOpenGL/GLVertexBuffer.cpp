#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexBuffer.h"

namespace DE
{
    void GLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
    }
    void GLVertexBuffer::UnBind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLVertexBuffer::GLVertexBuffer(uint32_t size, BufferUsage usage)
    {
        glCreateBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::GLVertexBuffer(const void* data, uint32_t size, BufferUsage usage)
    {
        glCreateBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::~GLVertexBuffer()
    {
        glDeleteBuffers(1, &m_BufferId);
    }

    const BufferLayout& GLVertexBuffer::GetLayout() const
    {
        return m_Layout;
    }
    void GLVertexBuffer::SetLayout(const BufferLayout& layout)
    {
        m_Layout = layout;
    }

    void GLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
    GLenum GLVertexBuffer::BufferUsafeToGLBufferUsage(BufferUsage usage)
    {
        switch (usage)
        {
            case BufferUsage::Static: return GL_STATIC_DRAW;
            case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
            default: return GL_STATIC_DRAW;
        }
    }

    //*----------------------------------------------------------------------------------

    GLIndexBuffer::GLIndexBuffer(const uint32_t* indices, uint32_t indices_amount) : _indices_amount(indices_amount)
    {
        glCreateBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, indices_amount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }
    GLIndexBuffer::~GLIndexBuffer()
    {
        glDeleteBuffers(1, &m_BufferId);
    }

    void GLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
    }
    void GLIndexBuffer::UnBind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t GLIndexBuffer::IndicesAmount() const
    {
        return _indices_amount;
    }

}  // namespace DE