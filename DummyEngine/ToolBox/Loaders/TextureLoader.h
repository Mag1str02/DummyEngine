#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class TextureLoader {
        LOG_AUTHOR(TextureLoader)
    public:
        static TryFuture<Ref<TextureData>> Load(const TextureAsset::LoadingProperties& props);

    private:
        static Result<Ref<TextureData>> DoLoad(const TextureAsset::LoadingProperties& props);
    };

}  // namespace DummyEngine