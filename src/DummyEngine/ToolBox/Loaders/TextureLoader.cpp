#include <stb_image.h>

#include "ToolBox/Loaders/TextureLoader.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{

    TextureData TextureLoader::LoadTexture(const Path& path)
    {
        int width, height, nrChannels;
        TextureData texture_data;
        unsigned char* stb_data;
        unsigned char* new_data;
        std::string format_s;

        stbi_set_flip_vertically_on_load(true);
        Logger::Stage("loading", "TextureLoader", path.string());

        stb_data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
        texture_data.width = width;
        texture_data.height = height;

        if (!stb_data)
        {
            Logger::Error("loading", "TextureLoader", "Couldn't load texture: (" + path.string() + ")");
            return TextureData(CreateRef<unsigned char*>(nullptr), 0, 0, TextureFormat::None);
        }

        new_data =
            (unsigned char*)malloc(sizeof(unsigned char) * texture_data.width * texture_data.height * nrChannels);
        memcpy(new_data, stb_data, sizeof(unsigned char) * texture_data.width * texture_data.height * nrChannels);

        stbi_image_free(stb_data);
        texture_data.data = CreateRef<unsigned char*>(new_data);

        switch (nrChannels)
        {
            case 1:
                format_s = "RED";
                texture_data.format = TextureFormat::RED;
                break;
            case 3:
                format_s = "RGB";
                texture_data.format = TextureFormat::RGB;
                break;
            case 4:
                format_s = "RGBA";
                texture_data.format = TextureFormat::RGBA;
                break;
            default:
                format_s = "NONE";
                texture_data.format = TextureFormat::None;
                break;
        }
        Logger::Info("loading",
                     "TextureLoader",
                     "Texture loaded successfully: (Format: " + format_s + ")(Path: " + path.string() + ")");
        return texture_data;
    }
}  // namespace DE