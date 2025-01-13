#include "BufferLayout.h"

#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    //*~~~BufferElement~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    BufferElement::BufferElement(BufferElementType type, bool normalized) : Type(type), Size(SizeOfElementType(type)), Normalized(normalized) {}
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
        switch (Type) {
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
        elements_(elements), type_(BufferLayoutType::Vertex), divisor_(divisor) {}

    void BufferLayout::SetLayoutType(BufferLayoutType type) {
        type_ = type;
        CalculateOffsetsAndStride();
    }
    void BufferLayout::CalculateOffsetsAndStride() {
        switch (type_) {
            case BufferLayoutType::Vertex: {
                U32 offset = 0;
                for (auto& element : elements_) {
                    element.Offset = offset;
                    offset += element.Size;
                }
                stride_ = offset;
                break;
            }
            case BufferLayoutType::Uniform: {
                U32 offset = 0;
                for (auto& element : elements_) {
                    switch (element.Type) {
                        case BufferElementType::Float:
                        case BufferElementType::Int: {
                            element.Offset = offset;
                            offset += element.Size;
                            break;
                        }
                        case BufferElementType::Float2:
                        case BufferElementType::Int2: {
                            if (offset % 8 != 0) {
                                offset += 4;
                            }
                            element.Offset = offset;
                            offset += element.Size;
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
                            element.Offset = offset;
                            offset += element.Size;
                            break;
                        }
                        default: break;
                    }
                }
                if (offset % 16 != 0) {
                    offset += 16 - offset % 16;
                }
                stride_ = offset;
                break;
            }
            default: break;
        }
    }

    std::vector<BufferElement>::iterator BufferLayout::begin() {
        return elements_.begin();
    }
    std::vector<BufferElement>::iterator BufferLayout::end() {
        return elements_.end();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
        return elements_.begin();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::end() const {
        return elements_.end();
    }
    const BufferElement& BufferLayout::operator[](U32 index) const {
        DE_ASSERT(index < elements_.size(), "Index out of bounce {} should be between [0,{})", index, elements_.size()); // already >= 0 because of U32
        return elements_[index];
    }

    U32 BufferLayout::GetStride() const {
        return stride_;
    }
    U32 BufferLayout::GetDivisor() const {
        return divisor_;
    }
}  // namespace DummyEngine