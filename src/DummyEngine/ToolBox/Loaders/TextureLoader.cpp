#include <stb_image.h>
#include <stb_image_write.h>

#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"

namespace DE
{
    TextureLoader::LoaderState TextureLoader::m_State;

    Ref<TextureData> TextureLoader::Load(const TextureLoadingProps& props)
    {
        unsigned char* stb_data;
        int            width, height, nrChannels;
        TextureFormat  format;
        std::string    format_s;

        m_State.m_CurrentData = CreateRef<TextureData>();

        stbi_set_flip_vertically_on_load(props.flip_uvs ? true : false);

        stb_data = stbi_load(props.path.string().c_str(), &width, &height, &nrChannels, 0);

        if (!stb_data)
        {
            LOG_ERROR("TextureLoader", "Failed to load texture ", RelativeToExecutable(props.path));
            return nullptr;
        }

        switch (nrChannels)
        {
            case 1:
                format_s = "RED";
                format   = TextureFormat::RED;
                break;
            case 3:
                format_s = "RGB";
                format   = TextureFormat::RGB;
                break;
            case 4:
                format_s = "RGBA";
                format   = TextureFormat::RGBA;
                break;
            default:
                format_s = "NONE";
                format   = TextureFormat::None;
                break;
        }

        m_State.m_CurrentData->SetData(stb_data, width, height, format);
        stbi_image_free(stb_data);

        LOG_INFO("TextureLoader", "Texture loaded ", RelativeToExecutable(props.path), " Format (", format_s, ")");
        return m_State.m_CurrentData;
    }
    void TextureLoader::Save(const Path& path, const Ref<TextureData> data)
    {
        switch (data->Format())
        {
            case TextureFormat::RGBA: {
                stbi_write_png(
                    path.string().c_str(), data->Width(), data->Height(), data->Channels(), data->Data(), data->Width() * data->Channels());
                break;
            }
            default: break;
        }
    }
}  // namespace DE