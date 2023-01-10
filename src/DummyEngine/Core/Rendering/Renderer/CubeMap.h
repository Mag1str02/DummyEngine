#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE
{
    class CubeMap
    {
    public:
        virtual ~CubeMap() = default;

        virtual void Bind() const = 0;
        static Ref<CubeMap> Create(const Ref<TextureData> texture_data);
    };
}  // namespace DE