#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    namespace fs = std::filesystem;

    class TextureLoader {
        LOGGER_AUTHOR(TextureLoader)
    public:
        static Ref<TextureData> Load(const TextureAsset::LoadingProperties& props);
        // static void             Save(const Path& path, const Ref<TextureData> data);
    };
}  // namespace DE