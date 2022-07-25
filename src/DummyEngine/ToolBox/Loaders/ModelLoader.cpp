#include <assimp/postprocess.h>

#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {
ModelLoader::ModelLoader() {
}

ModelLoader& ModelLoader::Get() {
    static ModelLoader model_loader;
    return model_loader;
}

void ModelLoader::ILoadModel(const Path& path, RenderModelData& data) {
    const aiScene* scene = m_Importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Error("loading", "ModelLoader", std::string("Failed To Load Scene: (" + path.string() + ")\n") + m_Importer.GetErrorString());
        return;
    }

    m_CurrentMeshId = 0;
    m_MeshesAmount = 0;
    m_NodesAmount = 0;
    m_VerticesAmount = 0;
    m_CurrentData = &data;
    m_CurrentDirectory = fs::canonical(path / "..");
    // Logger::Warning("loading", "ModelLoader", _directory.string());

    IReadModelProperties(scene->mRootNode, scene);
    data.meshes.resize(m_MeshesAmount);
    IProcessNode(scene->mRootNode, scene);

    Logger::Info("loading", "ModelLoader", "Model loaded: " + path.string());
    Logger::Info("loading", "ModelLoader",
                 "Model properties: Nodes - " + std::to_string(m_NodesAmount) + " | Meshes - " + std::to_string(m_MeshesAmount) + " | Vertices - " + std::to_string(m_VerticesAmount));
}
void ModelLoader::IProcessNode(aiNode* node, const aiScene* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        IProcessMesh(mesh, scene);
    }
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        IProcessNode(node->mChildren[i], scene);
    }
}
void ModelLoader::IProcessMesh(aiMesh* mesh, const aiScene* scene) {
    RenderMeshData& current_mesh = m_CurrentData->meshes[m_CurrentMeshId];
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        Vertex3D vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }
        current_mesh.vertices.push_back(vertex);
    }
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            current_mesh.indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        current_mesh.material.diffuse_color = IGetmaterialColor(material, ColorType::diffuse);
        current_mesh.material.diffuse_map = ILoadMaterialTexture(material, aiTextureType_DIFFUSE);
        current_mesh.material.specular_map = ILoadMaterialTexture(material, aiTextureType_SPECULAR);
    }
    ++m_CurrentMeshId;
}
Vec3 ModelLoader::IGetmaterialColor(aiMaterial* mat, ColorType type) {
    aiColor3D color(0.f, 0.f, 0.f);
    switch (type) {
        case ColorType::diffuse:
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            break;
        case ColorType::specular:
            mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
            break;
        case ColorType::ambient:
            mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
            break;
        default:
            break;
    }
    Vec3 res(color.r, color.g, color.b);
    return res;
}
TextureData ModelLoader::ILoadMaterialTexture(aiMaterial* mat, aiTextureType type) {
    TextureData texture_data;
    aiString file_name;
    Path texture_path;
    if (mat->GetTextureCount(type) > 1) {
        Logger::Warning("loading", "ModelLoader", "Model has more multiple textures of same type. Loading only first one.");
    }
    mat->GetTexture(type, 0, &file_name);
    // Logger::Warning("loading", "ModelLoader", file_name.C_Str());
    // Logger::Warning("loading", "ModelLoader", std::to_string(mat->GetTextureCount(type)));
    texture_path = m_CurrentDirectory / file_name.C_Str();
    if (m_PathToTextureData.find(texture_path.string()) == m_PathToTextureData.end()) {
        m_PathToTextureData[texture_path.string()] = TextureLoader::LoadTexture(texture_path);
    }
    texture_data = m_PathToTextureData[texture_path.string()];
    return texture_data;
}
void ModelLoader::IReadModelProperties(aiNode* node, const aiScene* scene) {
    ++m_NodesAmount;
    m_MeshesAmount += node->mNumMeshes;
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        m_VerticesAmount += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
    }
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        IReadModelProperties(node->mChildren[i], scene);
    }
}

void ModelLoader::LoadModel(const Path& path, RenderModelData& data) {
    Get().ILoadModel(path, data);
}
}  // namespace DE