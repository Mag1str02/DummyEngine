#include "texture.h"

Texture::Texture()
{
}
Texture::Texture(const fs::path &path_to_file, std::string type)
{
    Init(path_to_file, type);
}

bool Texture::Init(const fs::path &path_to_file, std::string type)
{
    _type = type;
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *texture_data = stbi_load(path_to_file.string().c_str(), &width, &height, &nrChannels, 0);

    if (!texture_data)
    {
        Logger::Error("loading", "Couldn't load texture: (" + path_to_file.string() + ")");
        return false;
    }
    std::string format_s;
    GLenum format;
    if (nrChannels == 1)
    {
        format_s = "RED";
        format = GL_RED;
    }
    else if (nrChannels == 3)
    {
        format_s = "RGB";
        format = GL_RGB;
    }
    else if (nrChannels == 4)
    {
        format_s = "RGBA";
        format = GL_RGBA;
    }

    glGenTextures(1, &_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture_data);

    Logger::Info("loading", "Texture loaded successfully: (Format: " + format_s + ")(" + path_to_file.string() + ")");
    return true;
}
void Texture::BindToUnit(size_t unit_id) const
{
    glActiveTexture(GL_TEXTURE0 + unit_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

void Texture::SetType(std::string type)
{
    _type = type;
}
std::string Texture::GetType() const
{
    return _type;
}
