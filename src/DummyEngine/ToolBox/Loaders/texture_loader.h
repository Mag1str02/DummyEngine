#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/render_data.h"

namespace DE {
namespace fs = std::filesystem;

class TextureLoader {
private:
    TextureLoader();
    static TextureLoader& Get();

    Texture2DData ILoadTexture2D(const fs::path& path);

public:
    static Texture2DData LoadTexture2D(const fs::path& path);
};
}  // namespace DE