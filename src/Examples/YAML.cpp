#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

struct TMP
{
    Mat4 mat4;
    Vec3 vec3;
    Vec2 vec2;
};

int main()
{
    LocalBuffer buffer;
    std::vector<TMP> normal_buffer(4);

    buffer.Allocate({BufferElement(BufferElementType::Mat4), BufferElement(BufferElementType::Float3), BufferElement(BufferElementType::Float2)}, 4);
    for (size_t i = 0; i < 4; ++i)
    {
        auto& node = normal_buffer[i];
        node.mat4 = Mat4(i + 1);
        node.vec3 = Vec3((i + 1) * 2);
        node.vec2 = Vec2((i + 1) * 3);
    }
    buffer.SetData(&normal_buffer[0]);
    for (size_t i = 0; i < 4; ++i)
    {
        auto node = buffer[i];
        std::cout << node.Get<Mat4>(0) << std::endl;
        std::cout << node.Get<Vec3>(1) << std::endl;
        std::cout << node.Get<Vec2>(2) << std::endl;
    }
}