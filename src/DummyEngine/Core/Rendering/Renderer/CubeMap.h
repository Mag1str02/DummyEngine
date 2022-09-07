#pragma once

#include "Addition/Base.h"
#include "Core/ResourceManaging/RawData.h"

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