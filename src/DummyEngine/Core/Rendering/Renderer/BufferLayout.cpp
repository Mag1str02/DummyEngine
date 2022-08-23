#include "Core/Rendering/Renderer/BufferLayout.h"

namespace DE
{

    //*~~~BufferElement~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BufferElement::BufferElement(BufferElementType type, bool normalized) : type(type), normalized(normalized), size(SizeOfElementType(type)) {}
    uint32_t BufferElement::SizeOfElementType(BufferElementType type)
    {
        switch (type)
        {
            case BufferElementType::Float: return 4;
            case BufferElementType::Float2: return 8;
            case BufferElementType::Float3: return 12;
            case BufferElementType::Float4: return 16;
            case BufferElementType::Int: return 4;
            case BufferElementType::Int2: return 8;
            case BufferElementType::Int3: return 12;
            case BufferElementType::Int4: return 16;
            case BufferElementType::Mat4: return 64;
            default: return 0;
        }
    }
    uint32_t BufferElement::ComponentCount() const
    {
        switch (type)
        {
            case BufferElementType::Float: return 1;
            case BufferElementType::Float2: return 2;
            case BufferElementType::Float3: return 3;
            case BufferElementType::Float4: return 4;
            case BufferElementType::Int: return 1;
            case BufferElementType::Int2: return 2;
            case BufferElementType::Int3: return 3;
            case BufferElementType::Int4: return 4;
            case BufferElementType::Mat4: return 4;
            default: return 0;
        }
    }

    //*~~~BufferLayout~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements, uint32_t divisor) : m_Elements(elements), m_Divisor(divisor)
    {
        CalculateOffsetsAndStride();
    }
    void BufferLayout::CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        for (auto& element : m_Elements)
        {
            element.offset = offset;
            offset += element.size;
        }
        m_Stride = offset;
    }

    std::vector<BufferElement>::iterator BufferLayout::begin()
    {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::iterator BufferLayout::end()
    {
        return m_Elements.end();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::begin() const
    {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::end() const
    {
        return m_Elements.end();
    }
    const BufferElement& BufferLayout::operator[](uint32_t index) const
    {
        DE_ASSERT(index >= 0 && index < m_Elements.size(), "Index out of bounce.");
        return m_Elements[index];
    }

    uint32_t BufferLayout::GetStride() const
    {
        return m_Stride;
    }
    uint32_t BufferLayout::GetDivisor() const
    {
        return m_Divisor;
    }
}  // namespace DE