#pragma once

#include <glm/glm.hpp>
#include <filesystem>

namespace DE
{

    using VoidFPtr    = void (*)();
    using ComponentId = int16_t;
    using EntityId    = int64_t;

    using Mat4                                   = glm::mat4;
    using Vec4                                   = glm::vec4;
    using Vec3                                   = glm::vec3;
    using Vec2                                   = glm::vec2;
    using Path                                   = std::filesystem::path;
    template <typename A, typename B> using Pair = std::pair<A, B>;

    namespace fs = std::filesystem;

    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };
    enum class ShaderPartType
    {
        None = 0,
        Vertex,
        Geometry,
        Fragment
    };
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
        Int4,
        Mat4
    };
    enum class BufferUsage
    {
        None = 0,
        Static,
        Dynamic
    };

    struct Vertex3D
    {
        Vec3 position;
        Vec3 normal;
        Vec2 tex_coords;
    };

}  // namespace DE