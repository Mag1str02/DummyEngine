#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"

namespace DummyEngine {
    namespace fs = std::filesystem;

    class ModelLoader {
    public:
        static TryFuture<Ref<RenderMeshData>> Load(const RenderMeshAsset::LoadingProperties& props);
    };  // namespace DummyEngine

}  // namespace DummyEngine