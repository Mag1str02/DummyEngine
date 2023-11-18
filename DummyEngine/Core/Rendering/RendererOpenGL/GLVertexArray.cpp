#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexArray.h"

namespace DE {

    GLVertexArray::GLVertexArray() : m_CurrentAtributeId(0) {
        glGenVertexArrays(1, &m_ArrayId);
    }
    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &m_ArrayId);
    }

    void GLVertexArray::Remove() {
        glDeleteVertexArrays(1, &m_ArrayId);
    }

    void GLVertexArray::Bind() const {
        glBindVertexArray(m_ArrayId);
    }
    void GLVertexArray::UnBind() const {
        glBindVertexArray(0);
    }

    void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) {
        glBindVertexArray(m_ArrayId);
        vertex_buffer->Bind();

        const auto& layout = vertex_buffer->GetLayout();
        for (const auto& element : layout) {
            switch (element.type) {
                case BufferElementType::Float:
                case BufferElementType::Float2:
                case BufferElementType::Float3:
                case BufferElementType::Float4: {
                    glEnableVertexAttribArray(m_CurrentAtributeId);
                    glVertexAttribPointer(m_CurrentAtributeId,
                                          element.ComponentCount(),
                                          GL_FLOAT,
                                          element.normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (void*)element.offset);
                    glVertexAttribDivisor(m_CurrentAtributeId, layout.GetDivisor());
                    ++m_CurrentAtributeId;
                    break;
                }

                case BufferElementType::Int:
                case BufferElementType::Int2:
                case BufferElementType::Int3:
                case BufferElementType::Int4: {
                    glEnableVertexAttribArray(m_CurrentAtributeId);
                    glVertexAttribIPointer(m_CurrentAtributeId, element.ComponentCount(), GL_INT, layout.GetStride(), (void*)element.offset);
                    glVertexAttribDivisor(m_CurrentAtributeId, layout.GetDivisor());
                    ++m_CurrentAtributeId;
                    break;
                }
                case BufferElementType::Mat4: {
                    U32 count = element.ComponentCount();
                    for (U8 i = 0; i < count; ++i) {
                        glEnableVertexAttribArray(m_CurrentAtributeId);
                        glVertexAttribPointer(m_CurrentAtributeId,
                                              count,
                                              GL_FLOAT,
                                              element.normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (void*)(element.offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_CurrentAtributeId, layout.GetDivisor());
                        ++m_CurrentAtributeId;
                    }
                    break;
                }
                default: break;
            }
        }
        m_VertexBuffers.push_back(vertex_buffer);
    }

    void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
        glBindVertexArray(m_ArrayId);
        index_buffer->Bind();

        m_IndexBuffer = index_buffer;
    }

    const std::vector<Ref<VertexBuffer>>& GLVertexArray::GetVertexBuffers() const {
        return m_VertexBuffers;
    }
    const Ref<IndexBuffer>& GLVertexArray::GetIndexBuffer() const {
        return m_IndexBuffer;
    }
}  // namespace DE