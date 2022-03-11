#pragma once

#include <map>

#include "../../Structs/Textures/Texture2D/texture_2D.h"

namespace fs = std::filesystem;

class TextureManager
{
private:
    std::map<std::string, Texture2D> _textures_2D;
    std::map<std::string, std::string> _file_to_name;

    TextureManager();
    static TextureManager &Get();

    bool IAddTexture2D(const fs::path &path_to_file, const std::string &name, Texture2DType type);

    const Texture2D *IGetTexure2D(const std::string &name) const;

public:
    static bool AddTexture2D(const fs::path &path_to_file, const std::string &name, Texture2DType type);

    static const Texture2D *GetTexure2D(const std::string &name);
};