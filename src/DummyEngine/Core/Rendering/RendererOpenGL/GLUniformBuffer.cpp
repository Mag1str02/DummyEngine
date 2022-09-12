#include "Core/Rendering/RendererOpenGL/GLUniformBuffer.h"

namespace DE
{
    GLUniformBuffer::GLUniformBuffer(const BufferLayout& layout, uint32_t size) : m_Layout(layout)
    {
        m_Layout.SetLayoutType(BufferLayoutType::Uniform);
        m_Size = size * m_Layout.GetStride();

        m_LocalBuffer.Allocate(m_Layout, size);

        glCreateBuffers(1, &m_BufferId);
        glNamedBufferData(m_BufferId, m_Size, nullptr, GL_DYNAMIC_DRAW);
    }
    GLUniformBuffer::~GLUniformBuffer() { glDeleteBuffers(1, &m_BufferId); }

    void GLUniformBuffer::Bind(uint32_t index) const { glBindBufferBase(GL_UNIFORM_BUFFER, index, m_BufferId); }
    LocalBufferNode GLUniformBuffer::at(uint32_t index) { return m_LocalBuffer.at(index); }
    void GLUniformBuffer::PushData() const { glNamedBufferSubData(m_BufferId, 0, m_Size, m_LocalBuffer.m_Data); }
    const BufferLayout& GLUniformBuffer::GetLayout() const { return m_Layout; }

}  // namespace DE