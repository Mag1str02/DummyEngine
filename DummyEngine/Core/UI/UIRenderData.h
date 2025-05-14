#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace DummyEngine {

struct UIVertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
};

struct UIRenderData {
    std::vector<UIVertex> Vertices;
    std::vector<uint32_t> Indices;
};

} // namespace DummyEngine 