#pragma once

#include "Addition/Base.h"
#include "Core/ResourceManaging/RawData.h"

namespace DE
{
    class TextureEditor
    {
    public:
        static Ref<TextureData> GetTexturePart(const Ref<TextureData> data, uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos);

        static Ref<TextureData> GetSkyBoxSide(const Ref<TextureData> data, CubeSide side);
    };
}  // namespace DE