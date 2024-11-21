#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class TextureLoader {
        LOG_AUTHOR(TextureLoader)
    public:
        static Ref<TextureData> Load(const TextureAsset::LoadingProperties& props);
    };

}  // namespace DummyEngine