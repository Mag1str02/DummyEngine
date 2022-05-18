#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../../../../Config/config.h"
#include "../../../ToolBox/Dev/Logger/logger.h"

namespace DE {

ShaderProgram::ShaderProgramManager::ShaderProgramManager() {
}

ShaderProgram::ShaderProgramManager& ShaderProgram::ShaderProgramManager::Get() {
    static ShaderProgramManager shader_program_manager;
    return shader_program_manager;
}

void ShaderProgram::ShaderProgramManager::Initialize() {
    _initialized = true;
}
void ShaderProgram::ShaderProgramManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

ShaderProgramId ShaderProgram::ShaderProgramManager::CreateShaderProgram() {
    ShaderProgramId shader_program_id = glCreateProgram();
    _reference_count[shader_program_id] = 1;
    return shader_program_id;
}
ShaderProgramId ShaderProgram::ShaderProgramManager::CreateInstance(ShaderProgramId shader_program_id) {
    if (shader_program_id == ShaderProgramId(-1)) {
        return ShaderProgramId(-1);
    }
    ++_reference_count[shader_program_id];
    return shader_program_id;
}
void ShaderProgram::ShaderProgramManager::DestroyInstance(ShaderProgramId shader_program_id) {
    if (!_initialized) {
        return;
    }
    if (shader_program_id == ShaderProgramId(-1)) {
        return;
    }
    --_reference_count[shader_program_id];
    if (_reference_count[shader_program_id] == 0) {
        DestroyShaderProgram(shader_program_id);
    }
}
void ShaderProgram::ShaderProgramManager::DestroyShaderProgram(ShaderProgramId shader_program_id) {
    glDeleteShader(shader_program_id);
    _reference_count.erase(shader_program_id);
}

ShaderProgram::ShaderProgram() {
    _shader_program_id = ShaderProgramId(-1);
}
ShaderProgram::ShaderProgram(const ShaderProgram& other) {
    _shader_program_id = ShaderProgramManager::Get().CreateInstance(other._shader_program_id);
}
ShaderProgram::ShaderProgram(ShaderProgram&& other) {
    _shader_program_id = ShaderProgramManager::Get().CreateInstance(other._shader_program_id);
}
ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
    if (&other == this) {
        return *this;
    }
    ShaderProgramManager::Get().DestroyInstance(_shader_program_id);
    _shader_program_id = ShaderProgramManager::Get().CreateInstance(other._shader_program_id);
    return *this;
}
ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    if (&other == this) {
        return *this;
    }
    ShaderProgramManager::Get().DestroyInstance(_shader_program_id);
    _shader_program_id = ShaderProgramManager::Get().CreateInstance(other._shader_program_id);
    return *this;
}

ShaderProgram::~ShaderProgram() {
    ShaderProgramManager::Get().DestroyInstance(_shader_program_id);
}
void ShaderProgram::Init() {
    _shader_program_id = ShaderProgramManager::Get().CreateShaderProgram();
    Logger::Info("loading", "ShaderProgram", "Shader program (" + std::to_string(_shader_program_id) + ") initialized.");
}
void ShaderProgram::SmartInit(const fs::path& path_to_file) {
    Init();
    AddShader((path_to_file / path_to_file.filename()).string() + ".vs", GL_VERTEX_SHADER);
    AddShader((path_to_file / path_to_file.filename()).string() + ".fs", GL_FRAGMENT_SHADER);
    LinkProgram();
}

void ShaderProgram::AddShader(Shader shader) {
    glAttachShader(_shader_program_id, shader._shader_id);
    _shaders.push_back(shader);
}
void ShaderProgram::AddShader(const fs::path& path_to_file, GLuint shader_type) {
    Shader shader(path_to_file, shader_type);
    glAttachShader(_shader_program_id, shader._shader_id);
    _shaders.push_back(shader);
}
void ShaderProgram::LinkProgram() {
    glLinkProgram(_shader_program_id);
    _shaders.clear();

    Check();
}
void ShaderProgram::Use() {
    glUseProgram(_shader_program_id);
}

void ShaderProgram::Check() {
    int success;
    char info_log[MAX_COMPILE_ERROR_LEN];
    glGetProgramiv(_shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shader_program_id, 512, NULL, info_log);
        Logger::Error("loading", "ShaderProgram", "Failed to link shader program (" + std::to_string(_shader_program_id) + ")\n" + info_log);
        throw std::exception();
    }
    Logger::Info("loading", "ShaderProgram", "Shader program (" + std::to_string(_shader_program_id) + ") linked successfully");
}

int ShaderProgram::PosOf(const std::string& uniform_name) {
    // Logger::Info("rendering", "Unifrom pos: " + std::to_string(glGetUniformLocation(_shader_program_id, uniform_name.c_str())) + " Name: " +
    // uniform_name + " SP id: " + std::to_string(_shader_program_id));
    return glGetUniformLocation(_shader_program_id, uniform_name.c_str());
}

void ShaderProgram::SetFloat(const std::string& uniform_name, float value) {
    Use();
    glUniform1f(PosOf(uniform_name), value);
}
void ShaderProgram::SetInt(const std::string& uniform_name, int value) {
    Use();
    glUniform1i(PosOf(uniform_name), value);
}
void ShaderProgram::SetVec4f(const std::string& uniform_name, float x, float y, float z, float w) {
    Use();
    glUniform4f(PosOf(uniform_name), x, y, z, w);
}
void ShaderProgram::SetVec4f(const std::string& uniform_name, glm::vec4 value) {
    Use();
    glUniform4f(PosOf(uniform_name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetMat4fv(const std::string& uniform_name, glm::mat4 value) {
    Use();
    glUniformMatrix4fv(PosOf(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetVec3f(const std::string& uniform_name, float x, float y, float z) {
    Use();
    glUniform3f(PosOf(uniform_name), x, y, z);
}
void ShaderProgram::SetVec3f(const std::string& uniform_name, glm::vec3 vec) {
    Use();
    glUniform3f(PosOf(uniform_name), vec.x, vec.y, vec.z);
}
void ShaderProgram::SetVec3i(const std::string& uniform_name, int x, int y, int z) {
    Use();
    glUniform3i(PosOf(uniform_name), x, y, z);
}
}  // namespace DE