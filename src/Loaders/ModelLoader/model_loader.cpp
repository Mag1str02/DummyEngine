#include "model_loader.h"

#include "../../Objects/Models/Mesh/mesh.h"
#include "../../Memory/TextureManager/texture_manager.h"

ModelLoader::ModelLoader()
{
}

ModelLoader &ModelLoader::Get()
{
    static ModelLoader model_loader;
    return model_loader;
}

void ModelLoader::IReadModel(const fs::path &path, std::vector<Mesh> &meshes)
{
    _current_mesh_id = 0;
    _current_buffer = &meshes;
    _meshes_amount = 0;
    _nodes_amount = 0;
    _verices_amount = 0;
    const aiScene *scene = _importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Logger::Error("loading", std::string("Failed To Load Scene: (" + path.string() + ")\n") + _importer.GetErrorString());
        return;
    }
    _directory = fs::canonical(path / "..").string();
    IReadModelProperties(scene->mRootNode, scene);
    meshes.resize(_meshes_amount);

    IProcessNode(scene->mRootNode, scene);

    Logger::Info("loading", "Model loaded: " + path.string());
    Logger::Info("loading", "Model properties: Nodes - " + std::to_string(_nodes_amount) + " | Meshes - " + std::to_string(_meshes_amount) +
                                " | Vertices - " + std::to_string(_verices_amount));
}
void ModelLoader::IProcessNode(aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        IProcessMesh(mesh, scene);
    }
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        IProcessNode(node->mChildren[i], scene);
    }
}
void ModelLoader::IProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    Mesh &current_mesh = _current_buffer->at(_current_mesh_id);
    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        if (mesh->mNormals != NULL)
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
        }
        current_mesh.vertices.push_back(vertex);
    }
    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            current_mesh.indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuse_maps = ILoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        current_mesh.textures.insert(current_mesh.textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        std::vector<Texture> specular_maps = ILoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        current_mesh.textures.insert(current_mesh.textures.end(), specular_maps.begin(), specular_maps.end());
    }
    _current_mesh_id++;
}
std::vector<Texture> ModelLoader::ILoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture = TextureManager::AddTexture(_directory + "\\" + str.C_Str());
        texture.SetType(typeName);
        textures.push_back(texture);
    }
    return textures;
}
void ModelLoader::IReadModelProperties(aiNode *node, const aiScene *scene)
{
    ++_nodes_amount;
    _meshes_amount += node->mNumMeshes;
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        _verices_amount += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
    }
    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        IReadModelProperties(node->mChildren[i], scene);
    }
}

void ModelLoader::ReadModel(const fs::path &path, std::vector<Mesh> &meshes)
{
    Get().IReadModel(path, meshes);
}