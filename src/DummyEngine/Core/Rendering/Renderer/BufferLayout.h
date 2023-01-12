#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE
{
    struct BufferElement
    {
        BufferElementType type;
        uint32_t          size;
        size_t            offset;
        bool              normalized;

        BufferElement() = default;
        BufferElement(BufferElementType type, bool normalized = false);

        uint32_t ComponentCount() const;

        static uint32_t SizeOfElementType(BufferElementType type);
    };

    enum class BufferLayoutType
    {
        None = 0,
        Vertex,
        Uniform
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements, uint32_t divisor = 0);

        std::vector<BufferElement>::iterator       begin();
        std::vector<BufferElement>::iterator       end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;
        const BufferElement&                       operator[](uint32_t index) const;

        uint32_t GetStride() const;
        uint32_t GetDivisor() const;
        void     SetLayoutType(BufferLayoutType type);

    private:
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> m_Elements;
        BufferLayoutType           m_Type;
        uint32_t                   m_Stride;
        uint32_t                   m_Divisor;
    };

}  // namespace DE