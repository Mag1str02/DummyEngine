#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/Resources/Texture.hpp"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    namespace fs = std::filesystem;

    class TextureLoader {
    public:
        static Ref<TextureResource> Load(Ref<TextureResource> resource,
                                         const Path&          path,
                                         const bool           flip_uv = false,
                                         const TextureFormat& format  = TextureFormat::U8);
        // static void             Save(const Path& path, const Ref<TextureData> data);
    };
}  // namespace DE