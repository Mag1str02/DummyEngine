#pragma once

#include <GLAD/glad.h>

#include <filesystem>
#include <string>

#include "../../../Addition/types.h"
namespace DE {
namespace fs = std::filesystem;

class Shader {
private:
    class ShaderManager {
    private:
        std::vector<ShaderId> _shader_id;
        std::vector<ReferenceCount> _ref_count;
        std::unordered_map<InstanceId, size_t> _instance_to_index;
        ShaderManager() {
        }

    public:
        static ShaderManager& Get() {
            static ShaderManager shader_manager;
            return shader_manager;
        }
        void Add() {
            _shader_id.push_back(ShaderId());
        }

        /*
        std::pair<InstanceId, ShaderId> CreateShader(GLenum type) {
            _shader_id.push_back(glCreateShader(type));
            _shader_id.push_back()
        }
        */
    };

    ShaderId _shader_id;

    std::string ReadShaderFile(const fs::path& path_to_file);
    void CheckShader(const fs::path& path_to_file);

public:
    Shader();
    Shader(const fs::path& path_to_file, GLenum type);

    void Init(const fs::path& path_to_file, GLenum type);

    unsigned int GetId() const;
};
}  // namespace DE