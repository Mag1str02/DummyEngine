#pragma once

#include "DummyEngine/Addition/base.h"

namespace DE {
    enum class BufferElementType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4
    };
    enum class BufferUsage
    {
        None = 0,
        Static,
        Dynamic
    };

    struct BufferElement {
        BufferElementType type;
        uint32_t size;
        size_t offset;
        bool normalized;

        BufferElement() = default;
        BufferElement(BufferElementType type, bool normalized = false);

        uint32_t ComponentCount() const;

        static uint32_t SizeOfElementType(BufferElementType type);
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements);

        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;

        uint32_t GetStride() const;

    private:
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> _elements;
        uint32_t _stride;
    };

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
        static Ref<VertexBuffer> Create(const void* data, uint32_t size,
                                        BufferUsage usage = BufferUsage::Static);
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
