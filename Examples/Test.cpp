#include "DummyEngine/Utils/DummyEngine.h"

using namespace DE;

int main()
{
    BufferLayout layout({BufferElementType::Float2, BufferElementType::Float2, BufferElementType::Float2, BufferElementType::Mat4}, 0);
    layout.SetLayoutType(BufferLayoutType::Vertex);

    for (auto& element : layout)
    {
        std::cout << element.offset << " " << std::endl;
    }
    std::cout << layout.GetStride() << " " << std::endl;
}