#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../../UnSorted/Config/config.h"
#include "../../UnSorted/Logger/logger.h"
#include "../Shader/shader.h"

ShaderProgram::ShaderProgram()
{
}
void ShaderProgram::Init(std::string name)
{
    _name = name;
    _sp_id = glCreateProgram();
    Logger::Info("loading", "Shader program (" + name + ") initialized with id: " + std::to_string(_sp_id));
}
void ShaderProgram::SmartInit(const fs::path &path_to_file)
{
    Init(path_to_file.filename().string());
    AddShader((path_to_file / path_to_file.filename()).string() + ".vs", GL_VERTEX_SHADER);
    AddShader((path_to_file / path_to_file.filename()).string() + ".fs", GL_FRAGMENT_SHADER);
    LinkProgram();
}
ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_sp_id);
}

void ShaderProgram::AddShader(const fs::path &path_to_file, GLuint shader_type)
{
    Shader shader(path_to_file, shader_type);
    glAttachShader(_sp_id, shader.GetId());
    _shader_ids.push_back(shader.GetId());
}
void ShaderProgram::LinkProgram()
{
    glLinkProgram(_sp_id);
    for (const auto &shader : _shader_ids)
    {
        glDeleteShader(shader);
    }
    _shader_ids.clear();
    Check();
}
void ShaderProgram::Use()
{
    glUseProgram(_sp_id);
}

void ShaderProgram::Check()
{
    int success;
    char info_log[MAX_COMPILE_ERROR_LEN];
    glGetProgramiv(_sp_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_sp_id, 512, NULL, info_log);
        Logger::Error("loading", "Failed to link shader program: " + _name + "\n" + info_log);
        throw std::exception();
    }
    Logger::Info("loading", "Shader program linked successfully: " + _name);
}

int ShaderProgram::PosOf(const std::string &uniform_name)
{
    //Logger::Info("rendering", "Unifrom pos: " + std::to_string(glGetUniformLocation(_sp_id, uniform_name.c_str())) + " Name: " + uniform_name + " SP id: " + std::to_string(_sp_id));
    return glGetUniformLocation(_sp_id, uniform_name.c_str());
}

void ShaderProgram::SetFloat(const std::string &uniform_name, float value)
{
    Use();
    glUniform1f(PosOf(uniform_name), value);
}
void ShaderProgram::SetInt(const std::string &uniform_name, int value)
{
    Use();
    glUniform1i(PosOf(uniform_name), value);
}
void ShaderProgram::SetVec4f(const std::string &uniform_name, float x, float y, float z, float w)
{
    Use();
    glUniform4f(PosOf(uniform_name), x, y, z, w);
}
void ShaderProgram::SetVec4f(const std::string &uniform_name, glm::vec4 value)
{
    Use();
    glUniform4f(PosOf(uniform_name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetMat4fv(const std::string &uniform_name, glm::mat4 value)
{
    Use();
    glUniformMatrix4fv(PosOf(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetVec3f(const std::string &uniform_name, float x, float y, float z)
{
    Use();
    glUniform3f(PosOf(uniform_name), x, y, z);
}
void ShaderProgram::SetVec3f(const std::string &uniform_name, glm::vec3 vec)
{
    Use();
    glUniform3f(PosOf(uniform_name), vec.x, vec.y, vec.z);
}
void ShaderProgram::SetVec3i(const std::string &uniform_name, int x, int y, int z)
{
    Use();
    glUniform3i(PosOf(uniform_name), x, y, z);
}

void ShaderProgram::SetMaterial(const std::string &uniform_name, Material value)
{
    Use();
    value.diffuse->BindToUnit(1);
    value.specular->BindToUnit(2);
    value.emmission->BindToUnit(3);
    SetInt(uniform_name + ".diffuse", 1);
    SetInt(uniform_name + ".specular", 2);
    SetInt(uniform_name + ".emmision", 3);
    SetFloat(uniform_name + ".shininess", value.shininess);
}
