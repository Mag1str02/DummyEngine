#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual U32 Width() const = 0;
        virtual U32 Height() const = 0;
        virtual U32 RendererId() const = 0;

        virtual void SetData(const void* data, U32 size) = 0;

        virtual void Bind(U32 slot) const = 0;

        static Ref<Texture> Create(U32 widht, U32 height, TextureChannels format = TextureChannels::RGBA);
        static Ref<Texture> Create(const TextureData& texture_data);
    };
}  // namespace DE