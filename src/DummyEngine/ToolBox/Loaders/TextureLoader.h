#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/RenderStructs.h"

namespace DE
{
    namespace fs = std::filesystem;

    class TextureLoader
    {
    public:
        static Ref<TextureData> Load(const Path& path);
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