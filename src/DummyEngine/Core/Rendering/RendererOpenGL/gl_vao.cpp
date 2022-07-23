#include "gl_vao.h"

namespace DE
{

    GLVertexArray::GLVertexArray() : _current_atribute_id(0)
    {
        glCreateVertexArrays(1, &_array_id);
    }
    GLVertexArray::~GLVertexArray()
    {
        glDeleteVertexArrays(1, &_array_id);
    }

    void GLVertexArray::Bind() const
    {
        glBindVertexArray(_array_id);
    }
    void GLVertexArray::UnBind() const
    {
        glBindVertexArray(0);
    }

    void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
    {
        glBindVertexArray(_array_id);
        vertex_buffer->Bind();

        const auto& layout = vertex_buffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(_current_atribute_id);
            switch (element.type)
            {
                case BufferElementType::Float:
                case BufferElementType::Float2:
                case BufferElementType::Float3:
                case BufferElementType::Float4:
                    glVertexAttribPointer(_current_atribute_id,
                                          element.ComponentCount(),
                                          GL_FLOAT,
                                          element.normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (void*)element.offset);

                case BufferElementType::Int:
                case BufferElementType::Int2:
                case BufferElementType::Int3:
                case BufferElementType::Int4:
                    glVertexAttribIPointer(_current_atribute_id,
                                           element.ComponentCount(),
                                           GL_INT,
                                           layout.GetStride(),
                                           (void*)element.offset);
            }
            _current_atribute_id++;
        }
        _vertex_buffers.push_back(vertex_buffer);
    }
    void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
    {
        glBindVertexArray(_array_id);
        index_buffer->Bind();

        _index_buffer = index_buffer;
    }

    const std::vector<Ref<VertexBuffer>>& GLVertexArray::GetVertexBuffers() const
    {
        return _vertex_buffers;
    }
    const Ref<IndexBuffer>& GLVertexArray::GetIndexBuffer() const
    {
        return _index_buffer;
    }

}  // namespace DE