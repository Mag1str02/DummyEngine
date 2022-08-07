#pragma once

#include "Addition/Base.h"

#include "Core/Rendering/Renderer/LocalBuffer.h"

namespace DE
{

   

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> Create(uint32_t size, BufferUsage usage = BufferUsage::Static);
        static Ref<VertexBuffer> Create(const void* data, uint32_t size, BufferUsage usage = BufferUsage::Static);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual uint32_t IndicesAmount() const = 0;

        static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
    };

}  // namespace DE
