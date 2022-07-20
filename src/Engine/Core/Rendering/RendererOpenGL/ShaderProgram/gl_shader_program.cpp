#include "gl_shader_program.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../../../../Addition/const.h"
#include "../../../../ToolBox/Dev/Logger/logger.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void GLShaderProgram::Init() {
    _shader_program_id = ShaderProgramManager::CreateShaderProgram();
    Logger::Info("loading", "GLShaderProgram", "GLShader program (" + std::to_string(_shader_program_id) + ") initialized.");
}
void GLShaderProgram::SmartInit(const fs::path& path_to_file) {
    Init();
    AddShader((path_to_file / path_to_file.filename()).string() + ".vs", GL_VERTEX_SHADER);
    AddShader((path_to_file / path_to_file.filename()).string() + ".fs", GL_FRAGMENT_SHADER);
    LinkProgram();
}

void GLShaderProgram::AddShader(GLShader shader) {
    glAttachShader(_shader_program_id, shader._shader_id);
    _shaders.push_back(shader);
}
void GLShaderProgram::AddShader(const fs::path& path_to_file, GLuint shader_type) {
    GLShader shader(path_to_file, shader_type);
    glAttachShader(_shader_program_id, shader._shader_id);
    _shaders.push_back(shader);
}
void GLShaderProgram::LinkProgram() {
    glLinkProgram(_shader_program_id);
    _shaders.clear();

    Check();
}
void GLShaderProgram::Use() {
    glUseProgram(_shader_program_id);
}

void GLShaderProgram::Check() {
    int success;
    char info_log[MAX_COMPILE_ERROR_LEN];
    glGetProgramiv(_shader_program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shader_program_id, 512, NULL, info_log);
        Logger::Fatal("loading", "GLShaderProgram", "Failed to link shader program (" + std::to_string(_shader_program_id) + ")\n" + info_log);
        throw std::exception();
    }
    Logger::Info("loading", "GLShaderProgram", "GLShader program (" + std::to_string(_shader_program_id) + ") linked successfully");
}

int GLShaderProgram::PosOf(const std::string& uniform_name) {
    // Logger::Info("rendering", "Unifrom pos: " + std::to_string(glGetUniformLocation(_shader_program_id, uniform_name.c_str())) + " Name: " +
    // uniform_name + " SP id: " + std::to_string(_shader_program_id));
    return glGetUniformLocation(_shader_program_id, uniform_name.c_str());
}
void GLShaderProgram::SetFloat(const std::string& uniform_name, float value) {
    Use();
    glUniform1f(PosOf(uniform_name), value);
}
void GLShaderProgram::SetInt(const std::string& uniform_name, int value) {
    Use();
    glUniform1i(PosOf(uniform_name), value);
}
void GLShaderProgram::SetVec4f(const std::string& uniform_name, float x, float y, float z, float w) {
    Use();
    glUniform4f(PosOf(uniform_name), x, y, z, w);
}
void GLShaderProgram::SetVec4f(const std::string& uniform_name, glm::vec4 value) {
    Use();
    glUniform4f(PosOf(uniform_name), value.x, value.y, value.z, value.w);
}
void GLShaderProgram::SetMat4fv(const std::string& uniform_name, glm::mat4 value) {
    Use();
    glUniformMatrix4fv(PosOf(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}
void GLShaderProgram::SetVec3f(const std::string& uniform_name, float x, float y, float z) {
    Use();
    glUniform3f(PosOf(uniform_name), x, y, z);
}
void GLShaderProgram::SetVec3f(const std::string& uniform_name, glm::vec3 vec) {
    Use();
    glUniform3f(PosOf(uniform_name), vec.x, vec.y, vec.z);
}
void GLShaderProgram::SetVec3i(const std::string& uniform_name, int x, int y, int z) {
    Use();
    glUniform3i(PosOf(uniform_name), x, y, z);
}
void GLShaderProgram::SetMaterial(const std::string& uniform_name, GLMaterial mat) {
    Use();
    glUniform3f(PosOf(uniform_name + ".ambient_color"), mat.ambient_color.x, mat.ambient_color.y, mat.ambient_color.z);
    glUniform3f(PosOf(uniform_name + ".diffuse_color"), mat.diffuse_color.x, mat.diffuse_color.y, mat.diffuse_color.z);
    glUniform3f(PosOf(uniform_name + ".specular_color"), mat.specular_color.x, mat.specular_color.y, mat.specular_color.z);
    glUniform1f(PosOf(uniform_name + ".shininess"), mat.shininess);
    glUniform1i(PosOf(uniform_name + ".specular_map"), 1);
    glUniform1i(PosOf(uniform_name + ".diffuse_map"), 2);
    glUniform1i(PosOf(uniform_name + ".normal_map"), 3);

    mat.specular_map.BindToUnit(1);
    mat.diffuse_map.BindToUnit(2);
    mat.normal_map.BindToUnit(3);
}

//*--------------------------------------------------

unsigned int GLShaderProgram::ShaderProgramManager::ICreateShaderProgram() {
    unsigned int shader_program_id = glCreateProgram();
    _reference_count[shader_program_id] = 1;
    return shader_program_id;
}
unsigned int GLShaderProgram::ShaderProgramManager::ICreateInstance(unsigned int shader_program_id) {
    if (shader_program_id == (unsigned int)-1) {
        return (unsigned int)-1;
    }
    ++_reference_count[shader_program_id];
    return shader_program_id;
}
void GLShaderProgram::ShaderProgramManager::IDestroyInstance(unsigned int shader_program_id) {
    if (!_initialized) {
        return;
    }
    if (shader_program_id == (unsigned int)-1) {
        return;
    }
    --_reference_count[shader_program_id];
    if (_reference_count[shader_program_id] == 0) {
        IDestroyShaderProgram(shader_program_id);
    }
}
void GLShaderProgram::ShaderProgramManager::IDestroyShaderProgram(unsigned int shader_program_id) {
    glDeleteShader(shader_program_id);
    _reference_count.erase(shader_program_id);
}

//*----------------------------------------------------------------------------------------------------

GLShaderProgram::GLShaderProgram() {
    _shader_program_id = (unsigned int)-1;
}
GLShaderProgram::GLShaderProgram(const GLShaderProgram& other) {
    _shader_program_id = ShaderProgramManager::CreateInstance(other._shader_program_id);
}
GLShaderProgram::GLShaderProgram(GLShaderProgram&& other) {
    _shader_program_id = ShaderProgramManager::CreateInstance(other._shader_program_id);
}
GLShaderProgram& GLShaderProgram::operator=(const GLShaderProgram& other) {
    if (&other == this) {
        return *this;
    }
    ShaderProgramManager::DestroyInstance(_shader_program_id);
    _shader_program_id = ShaderProgramManager::CreateInstance(other._shader_program_id);
    return *this;
}
GLShaderProgram& GLShaderProgram::operator=(GLShaderProgram&& other) {
    if (&other == this) {
        return *this;
    }
    ShaderProgramManager::DestroyInstance(_shader_program_id);
    _shader_program_id = ShaderProgramManager::CreateInstance(other._shader_program_id);
    return *this;
}
GLShaderProgram::~GLShaderProgram() {
    ShaderProgramManager::DestroyInstance(_shader_program_id);
}

//*--------------------------------------------------

GLShaderProgram::ShaderProgramManager::ShaderProgramManager() {
}
GLShaderProgram::ShaderProgramManager& GLShaderProgram::ShaderProgramManager::Get() {
    static ShaderProgramManager shader_program_manager;
    return shader_program_manager;
}

void GLShaderProgram::ShaderProgramManager::Initialize() {
    Get()._initialized = true;
}
void GLShaderProgram::ShaderProgramManager::Terminate() {
    Get()._initialized = false;
    Get()._reference_count.clear();
}

unsigned int GLShaderProgram::ShaderProgramManager::CreateShaderProgram() {
    return ShaderProgramManager::Get().ICreateShaderProgram();
}
unsigned int GLShaderProgram::ShaderProgramManager::CreateInstance(unsigned int shader_program_id) {
    return ShaderProgramManager::Get().ICreateInstance(shader_program_id);
}
void GLShaderProgram::ShaderProgramManager::DestroyInstance(unsigned int shader_program_id) {
    ShaderProgramManager::Get().IDestroyInstance(shader_program_id);
}
void GLShaderProgram::ShaderProgramManager::DestroyShaderProgram(unsigned int shader_program_id) {
    ShaderProgramManager::Get().IDestroyShaderProgram(shader_program_id);
}

}  // namespace DE