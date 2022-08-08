#include <stb_image.h>

#include "ToolBox/Loaders/TextureLoader.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{
    TextureLoader::LoaderState TextureLoader::m_State;

    Ref<TextureData> TextureLoader::Load(const Path& path)
    {
        int width, height, nrChannels;
        m_State.m_CurrentData = CreateRef<TextureData>();
        unsigned char* stb_data;
        unsigned char* new_data;
        std::string format_s;

        // stbi_set_flip_vertically_on_load(true);

        stb_data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        m_State.m_CurrentData->width = width;
        m_State.m_CurrentData->height = height;

        if (!stb_data)
        {
            Logger::Error("loading", "TextureLoader", "Failed to load texture " + RelativeToExecutable(path).string());
            return nullptr;
        }

        new_data = (unsigned char*)malloc(sizeof(unsigned char) * m_State.m_CurrentData->width * m_State.m_CurrentData->height * nrChannels);
        memcpy(new_data, stb_data, sizeof(unsigned char) * m_State.m_CurrentData->width * m_State.m_CurrentData->height * nrChannels);

        stbi_image_free(stb_data);
        m_State.m_CurrentData->data = CreateRef<unsigned char*>(new_data);

        switch (nrChannels)
        {
            case 1:
                format_s = "RED";
                m_State.m_CurrentData->format = TextureFormat::RED;
                break;
            case 3:
                format_s = "RGB";
                m_State.m_CurrentData->format = TextureFormat::RGB;
                break;
            case 4:
                format_s = "RGBA";
                m_State.m_CurrentData->format = TextureFormat::RGBA;
                break;
            default:
                format_s = "NONE";
                m_State.m_CurrentData->format = TextureFormat::None;
                break;
        }
        m_State.m_TextureDataByPath[fs::canonical(path)] = m_State.m_CurrentData;

        Logger::Info("loading", "TextureLoader", "Texture loaded " + RelativeToExecutable(path).string() + " Format (" + format_s + ")");
        return m_State.m_CurrentData;
    }
    Ref<TextureData> TextureLoader::Get(const Path& path)
    {
        Path canonitial;
        try
        {
            canonitial = fs::canonical(path);
        } catch (std::filesystem::__cxx11::filesystem_error)
        {
            Logger::Error("loading", "TextureLoader", "Texture file " + RelativeToExecutable(path).string() + " does not exist");
            return nullptr;
        }
        if (m_State.m_TextureDataByPath.find(canonitial) != m_State.m_TextureDataByPath.end())
        {
            return m_State.m_TextureDataByPath[canonitial];
        }
        else
        {
            return Load(path);
        }
    }
}  // namespace DE