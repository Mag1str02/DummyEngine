#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"

namespace DE
{

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual LocalBufferNode at(uint32_t index) = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;
        virtual void PushData()                               = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual uint32_t            Size() const      = 0;

        static Ref<VertexBuffer> Create(const BufferLayout& layout, uint32_t size, BufferUsage usage = BufferUsage::Static);
        static Ref<VertexBuffer> Create(const BufferLayout& layout, uint32_t size, const void* data, BufferUsage usage = BufferUsage::Static);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t IndicesAmount() const = 0;

        static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
    };

}  // namespace DE
