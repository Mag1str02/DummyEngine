#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class Shader {
private:
    unsigned int _shader_id;

    std::string ReadShaderFile(const fs::path& path_to_file);
    void CheckShader(const fs::path& path_to_file);

public:
    Shader();
    Shader(const fs::path& path_to_file, GLenum type);

    void Init(const fs::path& path_to_file, GLenum type);

    unsigned int GetId() const;
};