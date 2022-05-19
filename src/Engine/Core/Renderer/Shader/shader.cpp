#include "shader.h"

#include <fstream>

#include "../../../Addition/const.h"
#include "../../../ToolBox/Dev/Logger/logger.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

Shader::Shader(const fs::path& path_to_file, ::GLenum type) {
    Init(path_to_file, type);
}

void Shader::Init(const fs::path& path_to_file, ::GLenum type) {
    std::string source = ReadShaderFile(path_to_file);
    const char* source_c_str = source.c_str();

    _shader_id = ShaderManager::CreateShader(type);
    glShaderSource(_shader_id, 1, &source_c_str, NULL);
    glCompileShader(_shader_id);
    CheckShader(path_to_file);
}

std::string Shader::ReadShaderFile(const fs::path& path_to_file) {
    std::string line;
    std::string source_string;

    std::ifstream fin(path_to_file);
    if (!fin.is_open()) {
        Logger::Error("loading", "Shader", "Can't open shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    try {
        while (getline(fin, line)) {
            source_string.append(line + "\n");
        }
    } catch (...) {
        Logger::Error("loading", "Shader", "Failed to read shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    Logger::Info("loading", "Shader", "Shader source file readed: (" + path_to_file.string() + ")");
    return source_string;
}
void Shader::CheckShader(const fs::path& path_to_file) {
    int success = 1;
    char infoLog[MAX_COMPILE_ERROR_LEN];
    glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_shader_id, MAX_COMPILE_ERROR_LEN, NULL, infoLog);
        Logger::Error("loading", "Shader", "Failed to compile shader: (" + path_to_file.string() + ")\n" + std::string(infoLog));
        return;
    }
    Logger::Info("loading", "Shader", "Shader source file successfully compiled: (" + path_to_file.string() + ")");
}

//*--------------------------------------------------

ShaderId Shader::ShaderManager::ICreateShader(GLenum type) {
    ShaderId shader_id = glCreateShader(type);
    _reference_count[shader_id] = 1;
    return shader_id;
}
ShaderId Shader::ShaderManager::ICreateInstance(ShaderId shader_id) {
    if (shader_id == ShaderId(-1)) {
        return ShaderId(-1);
    }
    ++_reference_count[shader_id];
    return shader_id;
}
void Shader::ShaderManager::IDestroyInstance(ShaderId shader_id) {
    if (!_initialized) {
        return;
    }
    if (shader_id == ShaderId(-1)) {
        return;
    }
    --_reference_count[shader_id];
    if (_reference_count[shader_id] == 0) {
        IDestroyShader(shader_id);
    }
}
void Shader::ShaderManager::IDestroyShader(ShaderId shader_id) {
    glDeleteShader(shader_id);
    _reference_count.erase(shader_id);
}

//*----------------------------------------------------------------------------------------------------

Shader::Shader() {
    _shader_id = ShaderId(-1);
}
Shader::Shader(const Shader& other) {
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
}
Shader::Shader(Shader&& other) {
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
}
Shader& Shader::operator=(const Shader& other) {
    if (&other == this) {
        return *this;
    }
    ShaderManager::DestroyInstance(_shader_id);
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
    return *this;
}
Shader& Shader::operator=(Shader&& other) {
    if (&other == this) {
        return *this;
    }
    ShaderManager::DestroyInstance(_shader_id);
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
    return *this;
}
Shader::~Shader() {
    ShaderManager::DestroyInstance(_shader_id);
}

//*--------------------------------------------------
Shader::ShaderManager::ShaderManager() {
}
Shader::ShaderManager& Shader::ShaderManager::Get() {
    static ShaderManager shader_manager;
    return shader_manager;
}

void Shader::ShaderManager::Initialize() {
    Get()._initialized = true;
}
void Shader::ShaderManager::Terminate() {
    Get()._initialized = false;
    Get()._reference_count.clear();
}

ShaderId Shader::ShaderManager::CreateShader(GLenum type) {
    return ShaderManager::Get().ICreateShader(type);
}
ShaderId Shader::ShaderManager::CreateInstance(ShaderId shader_id) {
    return ShaderManager::Get().ICreateInstance(shader_id);
}
void Shader::ShaderManager::DestroyInstance(ShaderId shader_id) {
    ShaderManager::Get().IDestroyInstance(shader_id);
}
void Shader::ShaderManager::DestroyShader(ShaderId shader_id) {
    ShaderManager::Get().IDestroyShader(shader_id);
}

}  // namespace DE