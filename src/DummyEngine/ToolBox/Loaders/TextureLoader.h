#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"

namespace DE
{
    namespace fs = std::filesystem;

    class TextureLoader
    {
    public:
        static TextureData LoadTexture(const Path& path);
    };
}  // namespace DE