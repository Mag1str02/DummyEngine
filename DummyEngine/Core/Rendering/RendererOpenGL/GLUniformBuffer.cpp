#include "GLUniformBuffer.h"

namespace DummyEngine {

    GLUniformBuffer::GLUniformBuffer(const BufferLayout& layout, U32 size) : layout_(layout) {
        layout_.SetLayoutType(BufferLayoutType::Uniform);
        size_ = size * layout_.GetStride();

        local_buffer_.Allocate(layout_, size);

        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
        glBufferData(GL_UNIFORM_BUFFER, size_, nullptr, GL_DYNAMIC_DRAW);
    }
    GLUniformBuffer::~GLUniformBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

    void GLUniformBuffer::Bind(uint32_t index) const {
        glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer_id_);
    }
    LocalBufferNode GLUniformBuffer::At(uint32_t index) {
        return local_buffer_.At(index);
    }
    void GLUniformBuffer::PushData() const {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer_id_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, size_, local_buffer_.data_);
    }
    const BufferLayout& GLUniformBuffer::GetLayout() const {
        return layout_;
    }

}  // namespace DummyEngine