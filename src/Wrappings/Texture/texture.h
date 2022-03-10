#pragma once
#include <GLAD/glad.h>

#include <filesystem>
#include <string>

#include "../../../libs/STB_IMAGE/stb_image.h"
#include "../../UnSorted/Logger/logger.h"

namespace fs = std::filesystem;

class Texture
{
private:
    unsigned int _texture_id;

    std::string _type = "corrupted";

public:
    Texture();
    Texture(const fs::path &path_to_file, std::string type = "none");

    bool Init(const fs::path &path_to_file, std::string type = "none");
    void BindToUnit(size_t unit_id) const;

    void SetType(std::string type);
    std::string GetType() const;
};