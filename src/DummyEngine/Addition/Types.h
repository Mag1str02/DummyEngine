#pragma once

#include <glm/glm.hpp>
#include <filesystem>

namespace DE
{
    using Mat4 = glm::mat4;
    using Vec4 = glm::vec4;
    using Vec3 = glm::vec3;
    using Vec2 = glm::vec2;
    using Path = std::filesystem::path;

    namespace fs = std::filesystem;
}  // namespace DE