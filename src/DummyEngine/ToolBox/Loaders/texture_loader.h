#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/render_data.h"

namespace DE
{
    namespace fs = std::filesystem;

    class TextureLoader
    {
    public:
        static TextureData LoadTexture(const fs::path& path);
    };
}  // namespace DE