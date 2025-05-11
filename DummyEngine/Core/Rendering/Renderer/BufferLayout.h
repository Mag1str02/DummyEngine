#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <vector>

namespace DummyEngine {

    struct BufferElement {
    public:
        BufferElementType Type;
        U32               Size;
        size_t            Offset;
        bool              Normalized;

    public:
        BufferElement() = default;
        BufferElement(BufferElementType type, bool normalized = false);  // NOLINT

        U32 ComponentCount() const;

        static U32 SizeOfElementType(BufferElementType type);
    };

    enum class BufferLayoutType { None = 0, Vertex, Uniform };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements, U32 divisor = 0);

        std::vector<BufferElement>::iterator       begin();        // NOLINT
        std::vector<BufferElement>::iterator       end();          // NOLINT
        std::vector<BufferElement>::const_iterator begin() const;  // NOLINT
        std::vector<BufferElement>::const_iterator end() const;    // NOLINT
        const BufferElement&                       operator[](U32 index) const;

        U32  GetStride() const;
        U32  GetDivisor() const;
        void SetLayoutType(BufferLayoutType type);

    private:
        void CalculateOffsetsAndStride();

    private:
        std::vector<BufferElement> elements_;
        BufferLayoutType           type_;
        U32                        stride_;
        U32                        divisor_;
    };

}  // namespace DummyEngine