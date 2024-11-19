#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexBuffer.h"

namespace DE {
    void GLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
    }
    void GLVertexBuffer::UnBind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // TODO: Think about not creating LocalBuffer if BufferUsage is Static.

    GLVertexBuffer::GLVertexBuffer(const BufferLayout& layout, U32 size, BufferUsage usage) : m_Usage(usage), m_Layout(layout) {
        m_Layout.SetLayoutType(BufferLayoutType::Vertex);
        m_Size = (size * m_Layout.GetStride());

        if (m_Usage == BufferUsage::Dynamic) {
            m_LocalBuffer.Allocate(m_Layout, size);
        }
        glGenBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::GLVertexBuffer(const BufferLayout& layout, U32 size, const void* data, BufferUsage usage) : m_Usage(usage), m_Layout(layout) {
        m_Layout.SetLayoutType(BufferLayoutType::Vertex);
        m_Size = (size * m_Layout.GetStride());

        if (m_Usage == BufferUsage::Dynamic) {
            m_LocalBuffer.Allocate(m_Layout, size);
            m_LocalBuffer.SetData(data, m_Size);
        }

        glGenBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, m_Size, data, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &m_BufferId);
    }

    const BufferLayout& GLVertexBuffer::GetLayout() const {
        return m_Layout;
    }

    LocalBufferNode GLVertexBuffer::at(U32 index) {
        DE_ASSERT(m_Usage == BufferUsage::Dynamic, "Using at function on non-dynamic-usage vertex_buffer");
        return m_LocalBuffer.at(index);
    }
    void GLVertexBuffer::SetData(const void* data, U32 size) {
        if (m_Usage == BufferUsage::Dynamic) {
            m_LocalBuffer.SetData(data, size);
        } else {
            DE_ASSERT(m_Size == size, "Invalid data size {} expected {}", size, m_Size);

            glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }
    }
    void GLVertexBuffer::PushData() {
        DE_ASSERT(m_Usage == BufferUsage::Dynamic, "Using PushData function on non-dynamic-usage vertex_buffer. Use SetData instead");

        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_LocalBuffer.m_Size, m_LocalBuffer.m_Data);
    }

    GLenum GLVertexBuffer::BufferUsafeToGLBufferUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::Static: return GL_STATIC_DRAW;
            case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
            default: return GL_STATIC_DRAW;
        }
    }
    U32 GLVertexBuffer::Size() const {
        return m_Size / m_Layout.GetStride();
    }

    //*----------------------------------------------------------------------------------

    GLIndexBuffer::GLIndexBuffer(const U32* indices, U32 indices_amount) : _indices_amount(indices_amount) {
        glGenBuffers(1, &m_BufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ARRAY_BUFFER, indices_amount * sizeof(U32), indices, GL_STATIC_DRAW);
    }
    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &m_BufferId);
    }

    void GLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
    }
    void GLIndexBuffer::UnBind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    U32 GLIndexBuffer::IndicesAmount() const {
        return _indices_amount;
    }

}  // namespace DE