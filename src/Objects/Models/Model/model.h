#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "../../Cameras/FPSCamera/fps_camera.h"
#include "../Mesh/mesh.h"

namespace fs = std::filesystem;

class Model {
public:
    Model();
    Model(const fs::path& path_to_model);
    void Init(const fs::path& path_to_model);
    void Draw(const FPSCamera& camera, ShaderProgram& shader_program);

private:
    std::vector<Mesh> _meshes;
    std::string _directory;
    size_t _meshes_amount = 0;
    size_t _verices_amount = 0;
    size_t _node_amount = 0;

    void LoadModel(const fs::path& path_to_model);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};