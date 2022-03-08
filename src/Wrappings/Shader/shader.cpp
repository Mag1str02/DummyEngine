#include "shader.h"

#include <fstream>

#include "../../UnSorted/Config/config.h"
#include "../../UnSorted/Logger/logger.h"

std::string Shader::ReadShaderFile(const fs::path& path_to_file) {
    std::string line;
    std::string source_string;

    std::ifstream fin(path_to_file);
    if (!fin.is_open()) {
        Logger::Error("loading", "Can't open shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    try {
        while (getline(fin, line)) {
            source_string.append(line + "\n");
        }
    } catch (...) {
        Logger::Error("loading", "Failed to read shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    Logger::Info("loading", "Shader source file readed: (" + path_to_file.string() + ")");
    return source_string;
}
void Shader::CheckShader(const fs::path& path_to_file) {
    int success = 1;
    char infoLog[MAX_COMPILE_ERROR_LEN];
    glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_shader_id, MAX_COMPILE_ERROR_LEN, NULL, infoLog);
        Logger::Error("loading", "Failed to compile shader: (" + path_to_file.string() + ")\n" + std::string(infoLog));
        return;
    }
    Logger::Info("loading", "Shader source file successfully compiled: (" + path_to_file.string() + ")");
}

Shader::Shader() {
}
Shader::Shader(const fs::path& path_to_file, GLenum type) {
    Init(path_to_file, type);
}

void Shader::Init(const fs::path& path_to_file, GLenum type) {
    std::string source = ReadShaderFile(path_to_file);
    const char* source_c_str = source.c_str();

    _shader_id = glCreateShader(type);
    glShaderSource(_shader_id, 1, &source_c_str, NULL);
    glCompileShader(_shader_id);
    CheckShader(path_to_file);
}
unsigned int Shader::GetId() const {
    return _shader_id;
}