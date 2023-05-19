#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class TextureEditor {
    public:
        static Ref<TextureData> GetTexturePart(const Ref<TextureData> data, U32 width, U32 height, U32 x_pos, U32 y_pos);

        static Ref<TextureData> GetSkyBoxSide(const Ref<TextureData> data, CubeSide side);
    };
}  // namespace DE