#include "texture_loader.h"

#include "../../../libs/STB_IMAGE/stb_image.h"
#include "../../UnSorted/Logger/logger.h"

TextureLoader::TextureLoader() {
}
TextureLoader& TextureLoader::Get() {
    static TextureLoader texture_loader;
    return texture_loader;
}

Texture2D TextureLoader::ILoadTexture2D(const fs::path& path, Texture2DType texture_type) {
    unsigned int texture_id;
    unsigned char* texture_data;
    int width, height, nrChannels;
    std::string format_s;
    GLenum format;

    stbi_set_flip_vertically_on_load(true);
    texture_data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);

    if (!texture_data) {
        Logger::Error("loading", "TextureLoader", "Couldn't load texture: (" + path.string() + ")");
        return Texture2D(-1, Texture2DType::uninitialized);
    }
    switch (nrChannels) {
        case 1:
            format_s = "RED";
            format = GL_RED;
            break;
        case 3:
            format_s = "RGB";
            format = GL_RGB;
            break;
        case 4:
            format_s = "RGBA";
            format = GL_RGBA;
            break;
        default:
            format_s = "UNKNOWN";
            format = -1;
            break;
    }
    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture_data);

    Logger::Info("loading", "TextureLoader", "Texture loaded successfully: (Format: " + format_s + ")(Path: " + path.string() + ")");
    return Texture2D(texture_id, texture_type);
}

Texture2D TextureLoader::LoadTexture2D(const fs::path& path, Texture2DType texture_type) {
    return Get().ILoadTexture2D(path, texture_type);
}
