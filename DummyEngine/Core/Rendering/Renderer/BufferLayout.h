#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE
{
    struct BufferElement
    {
        BufferElementType type;
        U32          size;
        size_t            offset;
        bool              normalized;

        BufferElement() = default;
        BufferElement(BufferElementType type, bool normalized = false);

        U32 ComponentCount() const;

        static U32 SizeOfElementType(BufferElementType type);
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
        BufferLayout(std::initializer_list<BufferElement> elements, U32 divisor = 0);

        std::vector<BufferElement>::iterator       begin();
        std::vector<BufferElement>::iterator       end();
        std::vector<BufferElement>::const_iterator begin() const;
        std::vector<BufferElement>::const_iterator end() const;
        const BufferElement&                       operator[](U32 index) const;

        U32 GetStride() const;
        U32 GetDivisor() const;
        void     SetLayoutType(BufferLayoutType type);

    private:
        void CalculateOffsetsAndStride();

        std::vector<BufferElement> m_Elements;
        BufferLayoutType           m_Type;
        U32                   m_Stride;
        U32                   m_Divisor;
    };

}  // namespace DE