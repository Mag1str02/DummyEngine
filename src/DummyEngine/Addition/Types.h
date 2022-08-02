#pragma once

#include <glm/glm.hpp>
#include <filesystem>

namespace DE
{

    using ComponentId = int16_t;
    using EntityId = int64_t;

    using Mat4 = glm::mat4;
    using Vec4 = glm::vec4;
    using Vec3 = glm::vec3;
    using Vec2 = glm::vec2;
    using Path = std::filesystem::path;

    namespace fs = std::filesystem;

    enum class API
    {
        None = 0,
        OpenGL,
        Vulkan
    };
    enum class RenderSetting
    {
        None = 0,
        DepthTest,
        MultiSampling
    };
    enum class ShaderPartType
    {
        None = 0,
        Vertex,
        Geometry,
        Fragment
    };
    enum class TextureFormat
    {
        None = 0,
        RED,
        RGB,
        RGBA
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
        Int4
    };
    enum class BufferUsage
    {
        None = 0,
        Static,
        Dynamic
    };
    enum class WindowMode
    {
        none,
        windowed,
        borderless_windowed,
        fullscreen
    };

}  // namespace DE