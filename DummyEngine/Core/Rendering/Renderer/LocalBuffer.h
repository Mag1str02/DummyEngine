#pragma once

#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"

namespace DummyEngine {
    // TODO: Weak ptr stuff
    class LocalBufferNode;
    class LocalBuffer {
    public:
        LocalBuffer();
        ~LocalBuffer();

        void SetData(const void* data, U32 size);
        void Allocate(const BufferLayout& layout, U32 size);

        LocalBufferNode At(U32 index);

    private:
        void Allocate(U32 size);

        friend class GLVertexBuffer;
        friend class GLUniformBuffer;
        friend class LocalBufferNode;

        BufferLayout layout_;
        U8*          data_;
        U32          size_;
    };
    class LocalBufferNode {
    public:
        template <typename T> T& Get(U32 index) { return *(T*)(pointer_ + buffer_->layout_[index].Offset); }
        LocalBufferNode&         operator=(const LocalBufferNode& other);

    private:
        LocalBufferNode(LocalBuffer* buffer, U8* pointer);

        friend class LocalBuffer;

        LocalBuffer* buffer_;
        U8*          pointer_;
    };
}  // namespace DummyEngine