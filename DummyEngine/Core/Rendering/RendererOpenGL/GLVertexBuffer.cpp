#include "GLVertexBuffer.h"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    void GLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    }
    void GLVertexBuffer::UnBind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // TODO: Think about not creating LocalBuffer if BufferUsage is Static.

    GLVertexBuffer::GLVertexBuffer(const BufferLayout& layout, U32 size, BufferUsage usage) : usage_(usage), layout_(layout) {
        layout_.SetLayoutType(BufferLayoutType::Vertex);
        size_ = (size * layout_.GetStride());

        if (usage_ == BufferUsage::Dynamic) {
            local_buffer_.Allocate(layout_, size);
        }
        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, size_, nullptr, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::GLVertexBuffer(const BufferLayout& layout, U32 size, const void* data, BufferUsage usage) : usage_(usage), layout_(layout) {
        layout_.SetLayoutType(BufferLayoutType::Vertex);
        size_ = (size * layout_.GetStride());

        if (usage_ == BufferUsage::Dynamic) {
            local_buffer_.Allocate(layout_, size);
            local_buffer_.SetData(data, size_);
        }

        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, size_, data, BufferUsafeToGLBufferUsage(usage));
    }
    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

    const BufferLayout& GLVertexBuffer::GetLayout() const {
        return layout_;
    }

    LocalBufferNode GLVertexBuffer::At(U32 index) {
        DE_ASSERT(usage_ == BufferUsage::Dynamic, "Using at function on non-dynamic-usage vertex_buffer");
        return local_buffer_.At(index);
    }
    void GLVertexBuffer::SetData(const void* data, U32 size) {
        if (usage_ == BufferUsage::Dynamic) {
            local_buffer_.SetData(data, size);
        } else {
            DE_ASSERT(size_ == size, "Invalid data size {} expected {}", size, size_);

            glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }
    }
    void GLVertexBuffer::PushData() {
        DE_ASSERT(usage_ == BufferUsage::Dynamic, "Using PushData function on non-dynamic-usage vertex_buffer. Use SetData instead");

        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, local_buffer_.size_, local_buffer_.data_);
    }

    GLenum GLVertexBuffer::BufferUsafeToGLBufferUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::Static: return GL_STATIC_DRAW;
            case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
            default: return GL_STATIC_DRAW;
        }
    }
    U32 GLVertexBuffer::Size() const {
        return size_ / layout_.GetStride();
    }

    //*----------------------------------------------------------------------------------

    GLIndexBuffer::GLIndexBuffer(const U32* indices, U32 indices_amount) : indices_amount_(indices_amount) {
        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
        glBufferData(GL_ARRAY_BUFFER, indices_amount * sizeof(U32), indices, GL_STATIC_DRAW);
    }
    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &buffer_id_);
    }

    void GLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    }
    void GLIndexBuffer::UnBind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    U32 GLIndexBuffer::IndicesAmount() const {
        return indices_amount_;
    }

}  // namespace DummyEngine