#pragma once

#include "DummyEngine/Addition/Base.h"

namespace DE
{
    enum class TextureFormat
    {
        None = 0,
        RED,
        RGB,
        RGBA
    };
    struct TextureData
    {
        Ref<unsigned char*> data;
        uint32_t width;
        uint32_t height;
        TextureFormat format;
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;

        virtual void SetData(const void* data) = 0;

        virtual void Bind(uint32_t slot) const = 0;

        static Ref<Texture> Create(uint32_t widht, uint32_t height, TextureFormat format = TextureFormat::RGBA);
        static Ref<Texture> Create(const TextureData& texture_data);
    };
}  // namespace DE