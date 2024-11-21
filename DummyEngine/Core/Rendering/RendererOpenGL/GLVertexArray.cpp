#include "GLVertexArray.h"

namespace DummyEngine {

    GLVertexArray::GLVertexArray() : current_attribute_id_(0) {
        glGenVertexArrays(1, &array_id_);
    }
    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &array_id_);
    }

    void GLVertexArray::Bind() const {
        glBindVertexArray(array_id_);
    }
    void GLVertexArray::UnBind() const {
        glBindVertexArray(0);
    }

    void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) {
        glBindVertexArray(array_id_);
        vertex_buffer->Bind();

        const auto& layout = vertex_buffer->GetLayout();
        for (const auto& element : layout) {
            switch (element.Type) {
                case BufferElementType::Float:
                case BufferElementType::Float2:
                case BufferElementType::Float3:
                case BufferElementType::Float4: {
                    glEnableVertexAttribArray(current_attribute_id_);
                    glVertexAttribPointer(current_attribute_id_,
                                          element.ComponentCount(),
                                          GL_FLOAT,
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (void*)element.Offset);
                    glVertexAttribDivisor(current_attribute_id_, layout.GetDivisor());
                    ++current_attribute_id_;
                    break;
                }

                case BufferElementType::Int:
                case BufferElementType::Int2:
                case BufferElementType::Int3:
                case BufferElementType::Int4: {
                    glEnableVertexAttribArray(current_attribute_id_);
                    glVertexAttribIPointer(current_attribute_id_, element.ComponentCount(), GL_INT, layout.GetStride(), (void*)element.Offset);
                    glVertexAttribDivisor(current_attribute_id_, layout.GetDivisor());
                    ++current_attribute_id_;
                    break;
                }
                case BufferElementType::Mat4: {
                    U32 count = element.ComponentCount();
                    for (U8 i = 0; i < count; ++i) {
                        glEnableVertexAttribArray(current_attribute_id_);
                        glVertexAttribPointer(current_attribute_id_,
                                              count,
                                              GL_FLOAT,
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(current_attribute_id_, layout.GetDivisor());
                        ++current_attribute_id_;
                    }
                    break;
                }
                default: break;
            }
        }
        vertex_buffers_.push_back(vertex_buffer);
    }

    void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
        glBindVertexArray(array_id_);
        index_buffer->Bind();

        index_buffer_ = index_buffer;
    }

    const std::vector<Ref<VertexBuffer>>& GLVertexArray::GetVertexBuffers() const {
        return vertex_buffers_;
    }
    const Ref<IndexBuffer>& GLVertexArray::GetIndexBuffer() const {
        return index_buffer_;
    }
}  // namespace DummyEngine