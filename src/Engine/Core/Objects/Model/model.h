#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "../../Memory/VAO/vao.h"
#include "../../Structs/Meshes/mesh.h"
#include "../../Structs/Textures/Texture2D/texture_2d.h"
#include "../../Wrappings/ShaderProgram/shader_program.h"
#include "../Cameras/FPSCamera/fps_camera.h"

namespace DE {
namespace fs = std::filesystem;

class Model {
private:
    std::vector<VAO> _meshes;
    std::vector<std::vector<const Texture2D*>> _textures;

    void BindTextures(ShaderProgram& shader_program, size_t id) const;

public:
    Model();
    void AddMesh(const Mesh& mesh);
    void Draw(ShaderProgram& shader_program) const;
};
}  // namespace DE