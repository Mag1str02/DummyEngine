#include "texture_loader.h"
#include <windows.h>
#include <iostream>

#include "../../../libs/STB_IMAGE/stb_image.h"
#include "../../Dev/Logger/logger.h"

namespace DE {
TextureLoader::TextureLoader() {
}
TextureLoader& TextureLoader::Get() {
    static TextureLoader texture_loader;
    return texture_loader;
}

Texture2DData TextureLoader::ILoadTexture2D(const fs::path& path) {
    Texture2DData texture_data;
    unsigned char* stb_data;
    unsigned char* new_data;
    int nrChannels;
    std::string format_s;

    stbi_set_flip_vertically_on_load(true);
    Logger::Stage("loading", "TextureLoader", path.string());
    stb_data = stbi_load(path.string().c_str(), &texture_data.width, &texture_data.height, &nrChannels, 0);

    if (!stb_data) {
        Logger::Error("loading", "TextureLoader", "Couldn't load texture: (" + path.string() + ")");
        return Texture2DData(std::make_shared<unsigned char*>(nullptr), -1, -1, -1);
    }

    new_data = (unsigned char*)malloc(sizeof(unsigned char) * texture_data.width * texture_data.height * nrChannels);
    memcpy(new_data, stb_data, sizeof(unsigned char) * texture_data.width * texture_data.height * nrChannels);
    stbi_image_free(stb_data);
    texture_data.data = std::make_shared<unsigned char*>(new_data);

    switch (nrChannels) {
        case 1:
            format_s = "RED";
            texture_data.format = GL_RED;
            break;
        case 3:
            format_s = "RGB";
            texture_data.format = GL_RGB;
            break;
        case 4:
            format_s = "RGBA";
            texture_data.format = GL_RGBA;
            break;
        default:
            format_s = "UNKNOWN";
            break;
    }

    Logger::Info("loading", "TextureLoader", "Texture loaded successfully: (Format: " + format_s + ")(Path: " + path.string() + ")");
    return texture_data;
}

Texture2DData TextureLoader::LoadTexture2D(const fs::path& path) {
    return Get().ILoadTexture2D(path);
}
}  // namespace DE