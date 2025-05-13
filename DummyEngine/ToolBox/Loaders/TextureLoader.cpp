#include "TextureLoader.h"

#include "DummyEngine/Core/Application/Concurrency.h"
#include "DummyEngine/Core/Application/Config.h"

#include <stb_image.h>
#include <stb_image_write.h>

namespace DummyEngine {

    Result<Ref<TextureData>> TextureLoader::DoLoad(const TextureAsset::LoadingProperties& props) {
        DE_PROFILE_SCOPE("TextureLoader::DoLoad");

        void*           stb_data = nullptr;
        int             width = 0, height = 0, nr_channels = 0;
        TextureChannels channels;
        std::string     channels_s;
        std::string     format_s;

        auto res = CreateRef<TextureData>();
        stbi_set_flip_vertically_on_load(int(props.FlipUV));

        {
            DE_PROFILE_SCOPE("TextureLoader::DoLoad (stbi load)");
            switch (props.Format) {
                case TextureFormat::U8:
                    stb_data = stbi_load(props.Path.string().c_str(), &width, &height, &nr_channels, 0);
                    format_s = "U8";
                    break;
                case TextureFormat::Float:
                    stb_data = stbi_loadf(props.Path.string().c_str(), &width, &height, &nr_channels, 0);
                    format_s = "Float";
                    break;
                case TextureFormat::None: {
                    auto error = std::format("Texture was not loaded because of unspecified format (path='{}')", props.Path);
                    LOG_WARNING("{}", error);
                    return Results::Failure(error);
                }
                default: DE_ASSERT(false, "Unsupported texture format"); break;
            }
        }

        if (stb_data == nullptr) {
            auto error = std::format("Failed to load texture {}", Config::RelativeToExecutable(props.Path));
            LOG_ERROR("{}", error);
            return Results::Failure(error);
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

    TryFuture<Ref<TextureData>> TextureLoader::Load(const TextureAsset::LoadingProperties& props) {
        return Futures::Submit(Concurrency::GetEngineBackgroundScheduler(), [props]() { return DoLoad(props); });
    }

}  // namespace DummyEngine