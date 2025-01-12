#include "TextureLoader.h"

#include "DummyEngine/Core/Application/Config.h"

#include <stb_image.h>
#include <stb_image_write.h>

namespace DummyEngine {

    Ref<TextureData> TextureLoader::Load(const TextureAsset::LoadingProperties& props) {
        void*           stb_data;
        int             width, height, nr_channels;
        TextureChannels channels;
        std::string     channels_s;
        std::string     format_s;

        auto res = CreateRef<TextureData>();

        stbi_set_flip_vertically_on_load(int(props.FlipUV));

        switch (props.Format) {
            case TextureFormat::U8:
                stb_data = stbi_load(props.Path.string().c_str(), &width, &height, &nr_channels, 0);
                format_s = "U8";
                break;
            case TextureFormat::Float:
                stb_data = stbi_loadf(props.Path.string().c_str(), &width, &height, &nr_channels, 0);
                format_s = "Float";
                break;
            case TextureFormat::None: LOG_WARNING("Texture was not loaded because of unspecified format"); return res;
            default: DE_ASSERT(false, "Unsupported texture format"); break;
        }

        if (stb_data == nullptr) {
            LOG_ERROR("Failed to load texture {}", Config::RelativeToExecutable(props.Path));
            return nullptr;
        }

        switch (nr_channels) {
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

        res->SetData(stb_data, width, height, channels, props.Format);
        stbi_image_free(stb_data);

        LOG_INFO("Texture {} loaded, channels ({}), format ({})", Config::RelativeToExecutable(props.Path), channels_s, format_s);
        return res;
    }

}  // namespace DummyEngine