#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class CubeMap {
    public:
        virtual ~CubeMap()                  = default;
        virtual void   Bind(U32 slot) const = 0;
        virtual float& GetLOD()             = 0;

        static Ref<CubeMap> Create(const Ref<TextureData> texture_data);
        static Ref<CubeMap> Create(U32 size, Texture::Format format, Texture::Channels channels, bool gen_mipmap = false, bool depth_map = false);
    };
}  // namespace DE