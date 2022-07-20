#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <queue>
#include <string>

#include "../../../Initializer/initializer.h"

namespace DE {
namespace fs = std::filesystem;

class GLShader {
public:
    GLShader(const fs::path& path_to_file, ::GLenum type);
    void Init(const fs::path& path_to_file, ::GLenum type);

    GLShader();
    GLShader(const GLShader& other);
    GLShader(GLShader&& other);
    GLShader& operator=(const GLShader& other);
    GLShader& operator=(GLShader&& other);
    ~GLShader();

private:
    friend class Initializer;
    friend class GLShaderProgram;

    class ShaderManager {
    private:
        std::unordered_map<unsigned int, int64_t> _reference_count;
        bool _initialized;

        ShaderManager();

        unsigned int ICreateShader(GLenum type);
        unsigned int ICreateInstance(unsigned int shader_id);
        void IDestroyInstance(unsigned int shader_id);
        void IDestroyShader(unsigned int shader_id);

        static ShaderManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static unsigned int CreateShader(GLenum type);
        static unsigned int CreateInstance(unsigned int shader_id);
        static void DestroyInstance(unsigned int shader_id);
        static void DestroyShader(unsigned int shader_id);
    };

    unsigned int _shader_id;

    std::string ReadShaderFile(const fs::path& path_to_file);
    void CheckShader(const fs::path& path_to_file);
};
}  // namespace DE