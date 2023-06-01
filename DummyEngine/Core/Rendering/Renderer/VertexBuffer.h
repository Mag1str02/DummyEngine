#pragma once

#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual LocalBufferNode at(U32 index) = 0;

        virtual void SetData(const void* data, U32 size) = 0;
        virtual void PushData()                               = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual U32            Size() const      = 0;

        static Ref<VertexBuffer> Create(const BufferLayout& layout, U32 size, BufferUsage usage = BufferUsage::Static);
        static Ref<VertexBuffer> Create(const BufferLayout& layout, U32 size, const void* data, BufferUsage usage = BufferUsage::Static);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual U32 IndicesAmount() const = 0;

        static Ref<IndexBuffer> Create(const U32* indices, U32 count);
    };

}  // namespace DE
