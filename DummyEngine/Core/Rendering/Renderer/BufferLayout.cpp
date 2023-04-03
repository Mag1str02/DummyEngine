#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"

namespace DE {

    //*~~~BufferElement~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BufferElement::BufferElement(BufferElementType type, bool normalized) : type(type), normalized(normalized), size(SizeOfElementType(type)) {}
    U32 BufferElement::SizeOfElementType(BufferElementType type) {
        switch (type) {
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
    U32 BufferElement::ComponentCount() const {
        switch (type) {
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

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements, U32 divisor) :
        m_Elements(elements), m_Type(BufferLayoutType::Vertex), m_Divisor(divisor) {}

    void BufferLayout::SetLayoutType(BufferLayoutType type) {
        m_Type = type;
        CalculateOffsetsAndStride();
    }
    void BufferLayout::CalculateOffsetsAndStride() {
        switch (m_Type) {
            case BufferLayoutType::Vertex: {
                U32 offset = 0;
                for (auto& element : m_Elements) {
                    element.offset = offset;
                    offset += element.size;
                }
                m_Stride = offset;
                break;
            }
            case BufferLayoutType::Uniform: {
                U32 offset = 0;
                for (auto& element : m_Elements) {
                    switch (element.type) {
                        case BufferElementType::Float:
                        case BufferElementType::Int: {
                            element.offset = offset;
                            offset += element.size;
                            break;
                        }
                        case BufferElementType::Float2:
                        case BufferElementType::Int2: {
                            if (offset % 8 != 0) {
                                offset += 4;
                            }
                            element.offset = offset;
                            offset += element.size;
                            break;
                        }
                        case BufferElementType::Float3:
                        case BufferElementType::Int3:
                        case BufferElementType::Float4:
                        case BufferElementType::Int4:
                        case BufferElementType::Mat4: {
                            if (offset % 16 != 0) {
                                offset += 16 - offset % 16;
                            }
                            element.offset = offset;
                            offset += element.size;
                            break;
                        }
                        default: break;
                    }
                }
                if (offset % 16 != 0) {
                    offset += 16 - offset % 16;
                }
                m_Stride = offset;
                break;
            }
            default: break;
        }
    }

    std::vector<BufferElement>::iterator BufferLayout::begin() {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::iterator BufferLayout::end() {
        return m_Elements.end();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::end() const {
        return m_Elements.end();
    }
    const BufferElement& BufferLayout::operator[](U32 index) const {
        DE_ASSERT(index >= 0 && index < m_Elements.size(), "Index out of bounce (", index, ") should be between [0,", m_Elements.size(), ")");
        return m_Elements[index];
    }

    U32 BufferLayout::GetStride() const {
        return m_Stride;
    }
    U32 BufferLayout::GetDivisor() const {
        return m_Divisor;
    }
}  // namespace DE