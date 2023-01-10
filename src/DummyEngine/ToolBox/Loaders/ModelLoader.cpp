#include <assimp/postprocess.h>

#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE
{

    ModelLoader::LoaderState ModelLoader::m_State;

    Ref<RenderMeshData> ModelLoader::Load(const RenderMeshLoadingProps& properties)
    {
        const aiScene* scene = m_State.m_Importer.ReadFile(properties.path.string(), aiProcess_Triangulate | (properties.flip_uvs ? aiProcess_FlipUVs : 0));

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            Logger::Error("loading", "ModelLoader", "Failed to load model \"" + RelativeToExecutable(properties.path).string() + "\" Error (" + m_State.m_Importer.GetErrorString() + ")");
            return nullptr;
        }

        m_State.m_CurrentData = CreateScope<RenderMeshData>();

        m_State.m_CurrentMeshId = 0;
        m_State.m_MeshesAmount = 0;
        m_State.m_NodesAmount = 0;
        m_State.m_VerticesAmount = 0;
        m_State.m_CurrentDirectory = fs::canonical(properties.path / "..");

        ReadModelProperties(scene->mRootNode, scene);
        m_State.m_CurrentData->meshes.resize(m_State.m_MeshesAmount);
        ProcessNode(scene->mRootNode, scene);

        Logger::Info(
            "loading",
            "ModelLoader",
            "Model loaded " + RelativeToExecutable(properties.path).string() + " Meshes (" + std::to_string(m_State.m_MeshesAmount) + ") Vertices (" + std::to_string(m_State.m_VerticesAmount) + ")");
        if (properties.compress)
        {
            m_State.m_CurrentData->Compress();
        }

        return m_State.m_CurrentData;
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (size_t i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
        }
        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        RenderSubMeshData& current_mesh = m_State.m_CurrentData->meshes[m_State.m_CurrentMeshId];
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex3D vertex;

            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            if (mesh->mNormals)
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
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            current_mesh.material.diffuse_color = GetColor(material, ColorType::diffuse);
            current_mesh.material.ambient_color = GetColor(material, ColorType::ambient);
            current_mesh.material.specular_color = GetColor(material, ColorType::specular);
            current_mesh.material.diffuse_map = GetTexture(material, aiTextureType_DIFFUSE);
            current_mesh.material.specular_map = GetTexture(material, aiTextureType_SPECULAR);
            aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &current_mesh.material.shininess);
        }
        ++m_State.m_CurrentMeshId;
    }
    Vec3 ModelLoader::GetColor(aiMaterial* mat, ColorType type)
    {
        aiColor3D color(0.f, 0.f, 0.f);
        switch (type)
        {
            case ColorType::diffuse: mat->Get(AI_MATKEY_COLOR_DIFFUSE, color); break;
            case ColorType::specular: mat->Get(AI_MATKEY_COLOR_SPECULAR, color); break;
            case ColorType::ambient: mat->Get(AI_MATKEY_COLOR_AMBIENT, color); break;
            default: break;
        }
        Vec3 res(color.r, color.g, color.b);
        return res;
    }
    Ref<TextureData> ModelLoader::GetTexture(aiMaterial* mat, aiTextureType type)
    {
        aiString file_name;
        Path texture_path;
        if (mat->GetTextureCount(type) == 0)
        {
            return nullptr;
        }
        else if (mat->GetTextureCount(type) > 1)
        {
            Logger::Warning("loading", "ModelLoader", "Model has more multiple textures of same type. Loading only first one.");
        }
        mat->GetTexture(type, 0, &file_name);

        if (!m_State.m_ModelTextures.contains(m_State.m_CurrentDirectory / file_name.C_Str()))
        {
            m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()] = TextureLoader::Load({m_State.m_CurrentDirectory / file_name.C_Str(), false});
        }
        return m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()];
    }
    void ModelLoader::ReadModelProperties(aiNode* node, const aiScene* scene)
    {
        ++m_State.m_NodesAmount;
        m_State.m_MeshesAmount += node->mNumMeshes;
        for (size_t i = 0; i < node->mNumMeshes; ++i)
        {
            m_State.m_VerticesAmount += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        }
        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            ReadModelProperties(node->mChildren[i], scene);
        }
    }
}  // namespace DE