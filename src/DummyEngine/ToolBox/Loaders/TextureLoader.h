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
        static Ref<TextureData> Get(const Path& path);

    private:
        struct LoaderState
        {
            std::unordered_map<fs::path, Ref<TextureData>> m_TextureDataByPath;
            Ref<TextureData> m_CurrentData;
        };

        static LoaderState m_State;
    };
}  // namespace DE