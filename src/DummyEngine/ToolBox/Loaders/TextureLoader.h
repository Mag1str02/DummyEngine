#pragma once

#include "Addition/Base.h"
#include "Core/ResourceManaging/Assets.h"
#include "Core/ResourceManaging/RawData.h"

namespace DE
{
    namespace fs = std::filesystem;

    class TextureLoader
    {
    public:
        static Ref<TextureData> Load(const TextureLoadingProps& props);
        static void Save(const Path& path, const Ref<TextureData> data);

    private:
        struct LoaderState
        {
            Ref<TextureData> m_CurrentData;
        };

        static LoaderState m_State;
    };
}  // namespace DE