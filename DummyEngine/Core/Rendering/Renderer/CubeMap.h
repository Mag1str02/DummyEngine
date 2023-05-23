#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class CubeMap {
    public:
        virtual ~CubeMap()                = default;
        virtual void Bind(U32 slot) const = 0;

        static Ref<CubeMap> Create(const Ref<TextureData> texture_data);
        static Ref<CubeMap> Create(U32 size, TextureFormat format, TextureChannels channels);
    };
}  // namespace DE