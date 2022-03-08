#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class ShaderProgram {
private:
    unsigned int _sp_id;
    std::string _name;
    std::vector<unsigned int> _shader_ids;

    void Check();

public:
    ShaderProgram();
    void Init(std::string name);
    void SmartInit(const fs::path& path_to_file);
    ~ShaderProgram();

    void AddShader(const fs::path& path_to_file, GLuint shader_type);
    void LinkProgram();
    void Use();

    int PosOf(const std::string& filename);

    void SetFloat(const std::string& uniform_name, float value);
    void SetInt(const std::string& uniform_name, int value);
    void SetVec4f(const std::string& uniform_name, float x, float y, float z, float w);
    void SetVec4f(const std::string& uniform_name, glm::vec4 value);
    void SetVec3f(const std::string& uniform_name, float x, float y, float z);
    void SetVec3f(const std::string& uniform_name, glm::vec3 vec);
    void SetVec3i(const std::string& uniform_name, int x, int y, int z);
    void SetMat4fv(const std::string& uniform_name, glm::mat4 value);
};