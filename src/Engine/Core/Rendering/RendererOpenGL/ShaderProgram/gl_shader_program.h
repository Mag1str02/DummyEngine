#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../Shader/gl_shader.h"
#include "../../../Initializer/initializer.h"
#include "../RenderStructs/gl_render_structs.h"

namespace DE {
namespace fs = std::filesystem;

class GLShaderProgram {
public:
    void Init();
    void SmartInit(const fs::path& path_to_file);

    void AddShader(GLShader shader);
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
    void SetMaterial(const std::string& uniform_name, GLMaterial mat);

    GLShaderProgram();
    GLShaderProgram(const GLShaderProgram& other);
    GLShaderProgram(GLShaderProgram&& other);
    GLShaderProgram& operator=(const GLShaderProgram& other);
    GLShaderProgram& operator=(GLShaderProgram&& other);
    ~GLShaderProgram();

private:
    friend class Initializer;
    class ShaderProgramManager {
    private:
        bool _initialized;
        std::unordered_map<unsigned int, int64_t> _reference_count;

        ShaderProgramManager();

        unsigned int ICreateShaderProgram();
        unsigned int ICreateInstance(unsigned int shader_id);
        void IDestroyInstance(unsigned int shader_id);
        void IDestroyShaderProgram(unsigned int shader_id);

        static ShaderProgramManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static unsigned int CreateShaderProgram();
        static unsigned int CreateInstance(unsigned int shader_id);
        static void DestroyInstance(unsigned int shader_id);
        static void DestroyShaderProgram(unsigned int shader_id);
    };

    unsigned int _shader_program_id;
    std::vector<GLShader> _shaders;

    void Check();
    int PosOf(const std::string& filename);
};
}  // namespace DE