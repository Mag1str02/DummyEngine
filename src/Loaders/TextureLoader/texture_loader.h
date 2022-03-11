#pragma once
#include <GLAD/glad.h>

#include <filesystem>
#include <string>

#include "../../Structs/Textures/Texture2D/texture_2D.h"

namespace fs = std::filesystem;

class TextureLoader
{
private:
    TextureLoader();
    static TextureLoader &Get();

    Texture2D ILoadTexture2D(const fs::path &path, Texture2DType texture_type);

public:
    static Texture2D LoadTexture2D(const fs::path &path, Texture2DType texture_type);
};