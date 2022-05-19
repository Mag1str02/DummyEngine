#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../Shader/shader.h"

namespace DE {
namespace fs = std::filesystem;

class ShaderProgram {
public:
    void Init();
    void SmartInit(const fs::path& path_to_file);

    void AddShader(Shader shader);
    void AddShader(const fs::path& path_to_file, GLuint shader_type);
    void LinkProgram();
    void Use();

    void SetFloat(const std::string& uniform_name, float value);
    void SetInt(const std::string& uniform_name, int value);
    void SetVec4f(const std::string& uniform_name, float x, float y, float z, float w);
    void SetVec4f(const std::string& uniform_name, glm::vec4 value);
    void SetVec3f(const std::string& uniform_name, float x, float y, float z);
    void SetVec3f(const std::string& uniform_name, glm::vec3 vec);
    void SetVec3i(const std::string& uniform_name, int x, int y, int z);
    void SetMat4fv(const std::string& uniform_name, glm::mat4 value);

    ShaderProgram();
    ShaderProgram(const ShaderProgram& other);
    ShaderProgram(ShaderProgram&& other);
    ShaderProgram& operator=(const ShaderProgram& other);
    ShaderProgram& operator=(ShaderProgram&& other);
    ~ShaderProgram();

private:
    friend class Initializer;
    class ShaderProgramManager {
    private:
        bool _initialized;
        std::unordered_map<ShaderProgramId, ReferenceCount> _reference_count;

        ShaderProgramManager();

        ShaderProgramId ICreateShaderProgram();
        ShaderProgramId ICreateInstance(ShaderProgramId shader_id);
        void IDestroyInstance(ShaderProgramId shader_id);
        void IDestroyShaderProgram(ShaderProgramId shader_id);

        static ShaderProgramManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static ShaderProgramId CreateShaderProgram();
        static ShaderProgramId CreateInstance(ShaderProgramId shader_id);
        static void DestroyInstance(ShaderProgramId shader_id);
        static void DestroyShaderProgram(ShaderProgramId shader_id);
    };

    ShaderProgramId _shader_program_id;
    std::vector<Shader> _shaders;

    void Check();
    int PosOf(const std::string& filename);
};
}  // namespace DE