#include "LocalBuffer.h"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    //*~~~LocalBufferNode~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBufferNode::LocalBufferNode(LocalBuffer* buffer, U8* pointer) : buffer_(buffer), pointer_(pointer) {}
    LocalBufferNode& LocalBufferNode::operator=(const LocalBufferNode& other) {
        DE_ASSERT(buffer_ == other.buffer_, "Trying to assign LocalBufferNodes from diffrent LocalBuffers");
        std::memcpy(pointer_, other.pointer_, buffer_->layout_.GetStride());
        return *this;
    }

    //*~~~LocalBuffer~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBuffer::LocalBuffer() : data_(nullptr), size_(0) {}
    LocalBuffer::~LocalBuffer() {
        if (data_ != nullptr) {
            delete[] data_;
        }
    }
    LocalBufferNode LocalBuffer::At(U32 index) {
        return LocalBufferNode(this, data_ + index * layout_.GetStride());
    }
    void LocalBuffer::SetData(const void* data, U32 size) {
        DE_ASSERT(size_ == size, "Wrong data size {} expected {}", size, size_);
        memcpy(data_, data, size_);
    }
    void LocalBuffer::Allocate(const BufferLayout& layout, U32 size) {
        layout_ = layout;
        Allocate(size * layout_.GetStride());
    }
    void LocalBuffer::Allocate(U32 size) {
        if (data_ != nullptr) {
            delete[] data_;
        }
        data_ = (U8*)malloc(size);
        size_ = size;
    }

}  // namespace DummyEngine