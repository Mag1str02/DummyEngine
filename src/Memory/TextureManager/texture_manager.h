#pragma once

#include <map>

#include "../../Wrappings/Texture/texture.h"

namespace fs = std::filesystem;

class TextureManager {
private:
    std::map<std::string, Texture> textures;
    TextureManager();
    static TextureManager& GetManager();
    Texture IAddTexture(const fs::path& path_to_file);

public:
    static Texture AddTexture(const fs::path& path_to_file);
};