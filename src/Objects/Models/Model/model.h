#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "../../Cameras/FPSCamera/fps_camera.h"
#include "../Mesh/mesh.h"
#include "../../../Memory/VAO/vao.h"
#include "../../../Wrappings/ShaderProgram/shader_program.h"
#include "../../../Wrappings/Texture/texture.h"

namespace fs = std::filesystem;

class Model
{
private:
    std::vector<VAO> _meshes;
    std::vector<std::vector<Texture>> _textures;

    void BindTextures(ShaderProgram &shader_program, size_t id) const;

public:
    Model();
    void AddMesh(const Mesh &mesh);
    void Draw(ShaderProgram &shader_program) const;
};