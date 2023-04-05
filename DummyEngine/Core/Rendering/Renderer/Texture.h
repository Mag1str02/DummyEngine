#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual uint32_t RendererId() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot) const = 0;

        static Ref<Texture> Create(uint32_t widht, uint32_t height, TextureFormat format = TextureFormat::RGBA);
        static Ref<Texture> Create(const TextureData& texture_data);
    };
}  // namespace DE