#include "model.h"

#include <filesystem>
#include <string>

#include "../../../Memory/TextureManager/texture_manager.h"
#include "../../../UnSorted/Logger/logger.h"

Model::Model() {
}

Model::Model(const fs::path& path_to_model) {
    LoadModel(path_to_model);
}
void Model::Init(const fs::path& path_to_model) {
    LoadModel(path_to_model);
}

void Model::Draw(const FPSCamera& camera, ShaderProgram& shader_program) {
    shader_program.SetMat4fv("view", camera.GetViewMatrix());
    shader_program.SetMat4fv("projection", camera.GetProjectionMatrix());
    for (size_t i = 0; i < _meshes.size(); ++i)
        _meshes[i].Draw(shader_program);
}

void Model::LoadModel(const fs::path& path_to_model) {
    Logger::Info("loading", "Loadindg model: " + path_to_model.string());
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path_to_model.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Error("loading", std::string("Failed To Load Scene: ") + importer.GetErrorString());
        return;
    }
    _directory = fs::canonical(path_to_model / "..").string();

    ProcessNode(scene->mRootNode, scene);
    Logger::Info("loading", "Model loaded: " + path_to_model.string());
    Logger::Info("loading", "Model properties: Nodes - " + std::to_string(_node_amount) + " | Meshes - " + std::to_string(_meshes_amount) +
                                " | Vertices - " + std::to_string(_verices_amount));
}
void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    ++_node_amount;
    _meshes_amount += node->mNumMeshes;
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    _verices_amount += mesh->mNumVertices;
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals != NULL) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }
    unsigned int mn = 10000000, mx = 0;
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
            mn = std::min(mn, (unsigned int)face.mIndices[j]);
            mx = std::max(mx, (unsigned int)face.mIndices[j]);
        }
    }
    Logger::Warning("loading", "Mesh: " + std::to_string(_meshes_amount) + " Vertices: " + std::to_string(mesh->mNumVertices) + " Indices : (" +
                                   std::to_string(mn) + ", " + std::to_string(mx) + ")");
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}
std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

    std::vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture = TextureManager::AddTexture(_directory + "\\" + str.C_Str());
        texture.SetType(typeName);
        textures.push_back(texture);
    }
    return textures;
}
