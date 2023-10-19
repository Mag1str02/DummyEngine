#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

#include <stb_image.h>
#include <stb_image_write.h>

#include "DummyEngine/Core/Application/Config.h"

namespace DE {
    Ref<TextureResource> TextureLoader::Load(Ref<TextureResource> resource, const Path& path, const bool flip_uv = false, const TextureFormat &format = TextureFormat::U8) {
        void*           stb_data;
        int             width, height, nrChannels;
        TextureChannels channels;
        std::string     channels_s;
        std::string     format_s;
        stbi_set_flip_vertically_on_load(flip_uv ? true : false);

        switch (format) {
            case TextureFormat::U8:
                stb_data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
                format_s = "U8";
                break;
            case TextureFormat::Float:
                stb_data = stbi_loadf(path.string().c_str(), &width, &height, &nrChannels, 0);
                format_s = "Float";
                break;
            case TextureFormat::None: LOG_WARNING("TextureLoader", "Texture was not loaded because of unspecified format"); return res;
            default: DE_ASSERT(false, "Unsupported texture format"); break;
        }

        if (!stb_data) {
            LOG_ERROR("TextureLoader", "Failed to load texture (", RelativeToExecutable(path), ")");
            return nullptr;
        }

        switch (nrChannels) {
            case 1:
                channels_s = "RED";
                channels   = TextureChannels::RED;
                break;
            case 2:
                channels_s = "RG";
                channels   = TextureChannels::RG;
                break;
            case 3:
                channels_s = "RGB";
                channels   = TextureChannels::RGB;
                break;
            case 4:
                channels_s = "RGBA";
                channels   = TextureChannels::RGBA;
                break;
            default:
                channels_s = "NONE";
                channels   = TextureChannels::None;
                break;
        }
        resource->SetData(stb_data, width, height, channels, format);
        stbi_image_free(stb_data);

        LOG_INFO("TextureLoader", "Texture loaded (", RelativeToExecutable(path), ") channels (", channels_s, ") format (", format_s, ")");
        return resource;
    }
    // void TextureLoader::Save(const Path& path, const Ref<TextureData> data) {
    //     switch (data->Format()) {
    //         case TextureChannels::RGBA: {
    //             stbi_write_png(
    //                 path.string().c_str(), data->Width(), data->Height(), data->Channels(), data->Data(), data->Width() * data->Channels());
    //             break;
    //         }
    //         default: break;
    //     }
    // }
}  // namespace DE