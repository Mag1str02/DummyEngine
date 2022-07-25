#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/RenderStructs.h"

namespace DE
{
    namespace fs = std::filesystem;

    class TextureLoader
    {
    public:
        static TextureData LoadTexture(const Path& path);
    };
}  // namespace DE