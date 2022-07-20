#include "gl_shader.h"

#include <fstream>

#include "../../../../Addition/const.h"
#include "../../../../ToolBox/Dev/Logger/logger.h"

namespace DE::GLRenderer {

//*----------------------------------------------------------------------------------------------------

GLShader::GLShader(const fs::path& path_to_file, ::GLenum type) {
    Init(path_to_file, type);
}

void GLShader::Init(const fs::path& path_to_file, ::GLenum type) {
    std::string source = ReadShaderFile(path_to_file);
    const char* source_c_str = source.c_str();

    _shader_id = ShaderManager::CreateShader(type);
    glShaderSource(_shader_id, 1, &source_c_str, NULL);
    glCompileShader(_shader_id);
    CheckShader(path_to_file);
}

std::string GLShader::ReadShaderFile(const fs::path& path_to_file) {
    std::string line;
    std::string source_string;

    std::ifstream fin(path_to_file);
    if (!fin.is_open()) {
        Logger::Error("loading", "GLShader", "Can't open shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    try {
        while (getline(fin, line)) {
            source_string.append(line + "\n");
        }
    } catch (...) {
        Logger::Error("loading", "GLShader", "Failed to read shader source file: (" + path_to_file.string() + ")");
        return source_string;
    }
    Logger::Info("loading", "GLShader", "GLShader source file readed: (" + path_to_file.string() + ")");
    return source_string;
}
void GLShader::CheckShader(const fs::path& path_to_file) {
    int success = 1;
    char infoLog[MAX_COMPILE_ERROR_LEN];
    glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_shader_id, MAX_COMPILE_ERROR_LEN, NULL, infoLog);
        Logger::Error("loading", "GLShader", "Failed to compile shader: (" + path_to_file.string() + ")\n" + std::string(infoLog));
        return;
    }
    Logger::Info("loading", "GLShader", "GLShader source file successfully compiled: (" + path_to_file.string() + ")");
}

//*--------------------------------------------------

unsigned int GLShader::ShaderManager::ICreateShader(GLenum type) {
    unsigned int shader_id = glCreateShader(type);
    _reference_count[shader_id] = 1;
    return shader_id;
}
unsigned int GLShader::ShaderManager::ICreateInstance(unsigned int shader_id) {
    if (shader_id == (unsigned int)-1) {
        return (unsigned int)-1;
    }
    ++_reference_count[shader_id];
    return shader_id;
}
void GLShader::ShaderManager::IDestroyInstance(unsigned int shader_id) {
    if (!_initialized) {
        return;
    }
    if (shader_id == (unsigned int)-1) {
        return;
    }
    --_reference_count[shader_id];
    if (_reference_count[shader_id] == 0) {
        IDestroyShader(shader_id);
    }
}
void GLShader::ShaderManager::IDestroyShader(unsigned int shader_id) {
    glDeleteShader(shader_id);
    _reference_count.erase(shader_id);
}

//*----------------------------------------------------------------------------------------------------

GLShader::GLShader() {
    _shader_id = (unsigned int)-1;
}
GLShader::GLShader(const GLShader& other) {
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
}
GLShader::GLShader(GLShader&& other) {
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
}
GLShader& GLShader::operator=(const GLShader& other) {
    if (&other == this) {
        return *this;
    }
    ShaderManager::DestroyInstance(_shader_id);
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
    return *this;
}
GLShader& GLShader::operator=(GLShader&& other) {
    if (&other == this) {
        return *this;
    }
    ShaderManager::DestroyInstance(_shader_id);
    _shader_id = ShaderManager::CreateInstance(other._shader_id);
    return *this;
}
GLShader::~GLShader() {
    ShaderManager::DestroyInstance(_shader_id);
}

//*--------------------------------------------------
GLShader::ShaderManager::ShaderManager() {
}
GLShader::ShaderManager& GLShader::ShaderManager::Get() {
    static ShaderManager shader_manager;
    return shader_manager;
}

void GLShader::ShaderManager::Initialize() {
    Get()._initialized = true;
}
void GLShader::ShaderManager::Terminate() {
    Get()._initialized = false;
    Get()._reference_count.clear();
}

unsigned int GLShader::ShaderManager::CreateShader(GLenum type) {
    return ShaderManager::Get().ICreateShader(type);
}
unsigned int GLShader::ShaderManager::CreateInstance(unsigned int shader_id) {
    return ShaderManager::Get().ICreateInstance(shader_id);
}
void GLShader::ShaderManager::DestroyInstance(unsigned int shader_id) {
    ShaderManager::Get().IDestroyInstance(shader_id);
}
void GLShader::ShaderManager::DestroyShader(unsigned int shader_id) {
    ShaderManager::Get().IDestroyShader(shader_id);
}

}  // namespace DE