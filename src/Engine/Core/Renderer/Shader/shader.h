#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <queue>
#include <string>

#include "../../../Addition/types.h"
#include "../../Initializer/intitalizable.h"

namespace DE {
namespace fs = std::filesystem;

class Shader {
public:
    Shader(const fs::path& path_to_file, ::GLenum type);
    void Init(const fs::path& path_to_file, ::GLenum type);

    Shader();
    Shader(const Shader& other);
    Shader(Shader&& other);
    Shader& operator=(const Shader& other);
    Shader& operator=(Shader&& other);
    ~Shader();

private:
    friend class Initializer;
    friend class ShaderProgram;

    class ShaderManager {
    private:
        std::unordered_map<ShaderId, ReferenceCount> _reference_count;
        bool _initialized;

        ShaderManager();

        ShaderId ICreateShader(GLenum type);
        ShaderId ICreateInstance(ShaderId shader_id);
        void IDestroyInstance(ShaderId shader_id);
        void IDestroyShader(ShaderId shader_id);

        static ShaderManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static ShaderId CreateShader(GLenum type);
        static ShaderId CreateInstance(ShaderId shader_id);
        static void DestroyInstance(ShaderId shader_id);
        static void DestroyShader(ShaderId shader_id);
    };

    ShaderId _shader_id;

    std::string ReadShaderFile(const fs::path& path_to_file);
    void CheckShader(const fs::path& path_to_file);
};
}  // namespace DE