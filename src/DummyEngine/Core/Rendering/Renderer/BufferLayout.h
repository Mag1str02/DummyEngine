#pragma once

#include "Addition/Base.h"

namespace DE
{
    struct BufferElement
    {
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
        BufferLayout(std::initializer_list<BufferElement> elements, uint32_t divisor = 0);

        std::vector<BufferElement>::iterator begin();
        std::vector<BufferElement>::iterator end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;
        const BufferElement& operator[](uint32_t index) const;

        uint32_t GetStride() const;
        uint32_t GetDivisor() const;

    private:
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;
        uint32_t m_Divisor;
    };

}  // namespace DE