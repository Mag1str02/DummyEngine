#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <queue>
#include <string>

#include "../../../Addition/types.h"
#include "../../Initializer/intitalizable.h"

namespace DE
{
    namespace fs = std::filesystem;

    class Shader
    {
    private:
        friend class ShaderProgram;
        friend class Initializer;

        class ShaderManager : public Initializable
        {
        private:
            std::unordered_map<ShaderId, ReferenceCount> _reference_count;

            ShaderManager();
            void DestroyShader(ShaderId shader_id);

        public:
            static ShaderManager &Get();

            void Initialize() override;
            void Terminate() override;

            ShaderId CreateShader(GLenum type);
            ShaderId CreateInstance(ShaderId shader_id);
            void DestroyInstance(ShaderId shader_id);
        };

        ShaderId _shader_id;

        std::string ReadShaderFile(const fs::path &path_to_file);
        void CheckShader(const fs::path &path_to_file);

    public:
        Shader();
        Shader(const fs::path &path_to_file, ::GLenum type);
        Shader(const Shader &other);
        Shader(Shader &&other);
        Shader &operator=(const Shader &other);
        Shader &operator=(Shader &&other);
        ~Shader();

        void Init(const fs::path &path_to_file, ::GLenum type);
    };
} // namespace DE