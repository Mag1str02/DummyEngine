#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"

#include <assimp/postprocess.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/ResourceManaging/Resources/Mesh.hpp"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {
    Mat4 AssimpToGLM(const aiMatrix4x4& from) {
        Mat4 to;
        to[0][0] = from.a1;
        to[1][0] = from.a2;
        to[2][0] = from.a3;
        to[3][0] = from.a4;
        to[0][1] = from.b1;
        to[1][1] = from.b2;
        to[2][1] = from.b3;
        to[3][1] = from.b4;
        to[0][2] = from.c1;
        to[1][2] = from.c2;
        to[2][2] = from.c3;
        to[3][2] = from.c4;
        to[0][3] = from.d1;
        to[1][3] = from.d2;
        to[2][3] = from.d3;
        to[3][3] = from.d4;
        return to;
    }

    void ModelLoader::LoadBone(Bone& bone, aiNodeAnim* node) {
        for (S32 i = 0; i < node->mNumPositionKeys; ++i) {
            auto        pos        = node->mPositionKeys[i].mValue;
            float       time_stamp = node->mPositionKeys[i].mTime;
            KeyPosition data;
            data.position.x = pos.x;
            data.position.y = pos.y;
            data.position.z = pos.z;
            data.time_stamp = time_stamp;
            bone.m_Positions.push_back(data);
        }

        for (S32 i = 0; i < node->mNumRotationKeys; ++i) {
            auto        rot        = node->mRotationKeys[i].mValue;
            float       time_stamp = node->mRotationKeys[i].mTime;
            KeyRotation data;
            data.orientation.x = rot.x;
            data.orientation.y = rot.y;
            data.orientation.z = rot.z;
            data.orientation.w = rot.w;
            data.time_stamp    = time_stamp;
            bone.m_Rotations.push_back(data);
        }

        for (S32 i = 0; i < node->mNumScalingKeys; ++i) {
            aiVector3D scale      = node->mScalingKeys[i].mValue;
            float      time_stamp = node->mScalingKeys[i].mTime;
            KeyScale   data;
            data.scale.x    = scale.x;
            data.scale.y    = scale.y;
            data.scale.z    = scale.z;
            data.time_stamp = time_stamp;
            bone.m_Scales.push_back(data);
        }
        // LOG_INFO("ModelLoader",
        //          "Loaded bone (",
        //          bone.GetBoneName(),
        //          ", ",
        //          bone.GetBoneID(),
        //          ") with positions (",
        //          bone.m_Positions.size(),
        //          ") rotations (",
        //          bone.m_Rotations.size(),
        //          ") scales (",
        //          bone.m_Scales.size(),
        //          ")");
    }

    void ModelLoader::ReadWeights(aiMesh* mesh) {
        auto& model        = *m_State.m_CurrentData;
        auto& current_mesh = model.meshes[m_State.m_CurrentMeshId];
        if (!model.animation) {
            return;
        }
        auto& animation = *model.animation;

        for (int i = 0; i < mesh->mNumBones; ++i) {
            int         bone_id   = -1;
            std::string bone_name = mesh->mBones[i]->mName.C_Str();
            if (!animation.m_BoneNameToID.contains(bone_name)) {
                bone_id                             = animation.m_Bones.size();
                animation.m_BoneNameToID[bone_name] = bone_id;

                BoneInfo bone_info;
                bone_info.bone   = Bone(bone_name, bone_id);
                bone_info.offset = AssimpToGLM(mesh->mBones[i]->mOffsetMatrix);
                animation.m_Bones.push_back(bone_info);
                // printf("Added bone (%s) with id (%d)\n", bone_name.c_str(), bone_id);
            } else {
                bone_id = animation.m_BoneNameToID[bone_name];
            }
            if (bone_id == -1) {
                LOG_WARNING("ModelLoader", "Wrong bone id");
                return;
            }
            auto weights        = mesh->mBones[i]->mWeights;
            int  weights_amount = mesh->mBones[i]->mNumWeights;

            for (int j = 0; j < weights_amount; ++j) {
                int   v_id   = weights[j].mVertexId;
                float weight = weights[j].mWeight;

                if (v_id >= current_mesh.vertices.size()) {
                    LOG_WARNING("ModelLoader", "Wrong vertex id");
                    return;
                }
                current_mesh.vertices[v_id].AddBone(bone_id, weight);
            }
        }
    }
    void ModelLoader::ReadBones(Animation& animation, const aiAnimation* anim) {
        for (int i = 0; i < anim->mNumChannels; ++i) {
            auto channel   = anim->mChannels[i];
            auto bone_info = animation.GetBone(channel->mNodeName.data);
            if (!bone_info) {
                BoneInfo info;
                info.offset                                       = glm::mat4(1.0);
                animation.m_BoneNameToID[channel->mNodeName.data] = animation.m_Bones.size();
                animation.m_Bones.push_back(info);
                bone_info = &animation.m_Bones.back();
                LOG_WARNING("ModelLoader", "Added unknown bone");
            }
            LoadBone(bone_info->bone, channel);
        }
    }
    void ModelLoader::ReadAnimationNode(Animation::Node& node, const aiNode* src) {
        node.name           = src->mName.data;
        node.transformation = AssimpToGLM(src->mTransformation);
        node.childrens.resize(src->mNumChildren);

        for (int i = 0; i < src->mNumChildren; i++) {
            Animation::Node& data = node.childrens[i];
            ReadAnimationNode(data, src->mChildren[i]);
        }
    }
    void ModelLoader::ReadAnimation(Animation& animation, const aiScene* scene) {
        auto anim                  = scene->mAnimations[0];
        animation.m_Duration       = anim->mDuration;
        animation.m_TicksPerSecond = anim->mTicksPerSecond;
        ReadAnimationNode(animation.m_RootNode, scene->mRootNode);
        ReadBones(animation, anim);
    }

    ModelLoader::LoaderState ModelLoader::m_State;

    Ref<Material> ModelLoader::LoadMaterial(aiMaterial* mat) {
        Ref<Material> material = CreateRef<Material>();
        material->diffuse      = GetColor(mat, ColorType::Diffuse);
        material->specular     = GetColor(mat, ColorType::Specular);
        material->ambient      = GetColor(mat, ColorType::Ambient);
        material->albedo       = GetColor(mat, ColorType::Albedo);
        material->orm          = GetColor(mat, ColorType::ORM);
        material->emission     = GetColor(mat, ColorType::Emission);
        aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &material.shininess);

        material->albedo_map   = GetTexture(mat, aiTextureType_DIFFUSE);
        material->normal_map   = GetTexture(mat, aiTextureType_NORMALS);
        material->orm_map      = GetTexture(mat, aiTextureType_METALNESS);
        material->diffuse_map  = GetTexture(mat, aiTextureType_DIFFUSE);
        material->specular_map = GetTexture(mat, aiTextureType_SPECULAR);
        material->emission_map = GetTexture(mat, aiTextureType_EMISSIVE);
        return material;
    }

    void ModelLoader::Load(Path path, bool flip_uvs, bool compress, std::vector<SubMesh>& submeshes, Ref<Animation>& animation) {
        unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        if (flip_uvs) {
            flags |= aiProcess_FlipUVs;
        }
        const aiScene* scene = m_State.m_Importer.ReadFile(path.string(), flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERROR(
                "ModelLoader", "Failed to load model (", RelativeToExecutable(path), ") with error (", m_State.m_Importer.GetErrorString(), ")");
            return;
        }

        m_State.m_CurrentData = CreateRef<RenderMeshData>();
        if (scene->mNumAnimations > 0) {
            animation = CreateRef<Animation>();
        }

        m_State.m_CurrentMeshId    = 0;
        m_State.m_MeshesAmount     = 0;
        m_State.m_NodesAmount      = 0;
        m_State.m_VerticesAmount   = 0;
        m_State.m_CurrentDirectory = path.parent_path();

        ReadModelProperties(scene->mRootNode, scene);
        submeshes.resize(m_State.m_MeshesAmount);
        ProcessNode(scene->mRootNode, scene);
        if (animation) {
            ReadAnimation(*animation, scene);
        }
        LOG_INFO("ModelLoader",
                 "Model loaded (",
                 RelativeToExecutable(path),
                 ") with (",
                 m_State.m_MeshesAmount,
                 ") meshes, (",
                 m_State.m_VerticesAmount,
                 ") verticies");
        return;
    }

    Ref<RenderMeshData> ModelLoader::Load(const RenderMeshAsset::LoadingProperties& properties) {
        m_State.m_Props    = properties;
        unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        if (properties.flip_uvs) {
            flags |= aiProcess_FlipUVs;
        }
        const aiScene* scene = m_State.m_Importer.ReadFile(properties.path.string(), flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERROR("ModelLoader",
                      "Failed to load model (",
                      RelativeToExecutable(properties.path),
                      ") with error (",
                      m_State.m_Importer.GetErrorString(),
                      ")");
            return nullptr;
        }

        if (scene->mNumAnimations > 0) {
            m_State.m_Animation = CreateRef<Animation>();
        }
        m_State.m_CurrentMeshId    = 0;
        m_State.m_MeshesAmount     = 0;
        m_State.m_NodesAmount      = 0;
        m_State.m_VerticesAmount   = 0;
        m_State.m_CurrentDirectory = properties.path.parent_path();
        m_State.m_

        ReadModelProperties(scene->mRootNode, scene);
        m_State.m_CurrentData->meshes.resize(m_State.m_MeshesAmount);
        ProcessNode(scene->mRootNode, scene);
        if (m_State.m_CurrentData->animation) {
            ReadAnimation(*m_State.m_CurrentData->animation, scene);
        }
        if (properties.compress) {
            m_State.m_CurrentData->Compress();
        }
        LOG_INFO("ModelLoader",
                 "Model loaded (",
                 RelativeToExecutable(properties.path),
                 ") with (",
                 m_State.m_MeshesAmount,
                 ") meshes, (",
                 m_State.m_VerticesAmount,
                 ") verticies");
        return m_State.m_CurrentData;
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene) {
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex3D> vertices;
        std::vector<U32>      indices;
        Ref<Material>         mat;
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
            if (mesh->mTangents) {
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;
            }
            if (mesh->mTextureCoords[0]) {
                vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
                vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
            }
            vertices.push_back(vertex);
        }
        ReadWeights(mesh);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            mat                  = LoadMaterial(material);
        }
        m_State.m_SubMeshes.push_back(CreateRef<SubMesh>(vertices, indices, mat));
        ++m_State.m_CurrentMeshId;
    }
    Vec3 ModelLoader::GetColor(aiMaterial* mat, ColorType type) {
        aiColor3D color(1.f, 1.f, 1.f);
        switch (type) {
            case ColorType::Diffuse: mat->Get(AI_MATKEY_COLOR_DIFFUSE, color); break;
            case ColorType::Specular: mat->Get(AI_MATKEY_COLOR_SPECULAR, color); break;
            case ColorType::Ambient: mat->Get(AI_MATKEY_COLOR_AMBIENT, color); break;
            case ColorType::Emission: mat->Get(AI_MATKEY_COLOR_EMISSIVE, color); break;
            case ColorType::ORM:
                mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, color.g);
                mat->Get(AI_MATKEY_METALLIC_FACTOR, color.b);
                break;
            default: break;
        }
        Vec3 res(color.r, color.g, color.b);
        return res;
    }
    Ref<TextureResource> ModelLoader::GetTexture(aiMaterial* mat, aiTextureType type) {
        aiString file_name;
        Path     texture_path;
        if (mat->GetTextureCount(type) == 0) {
            return nullptr;
        } else if (mat->GetTextureCount(type) > 1) {
            LOG_WARNING("ModelLoader", "Model has more multiple textures of same type. Loading only first one.");
        }
        mat->GetTexture(type, 0, &file_name);

        if (!m_State.m_ModelTextures.contains(m_State.m_CurrentDirectory / file_name.C_Str())) {
            m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()] =
                TextureLoader::Load(m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()],
                                    m_State.m_CurrentDirectory / file_name.C_Str(),
                                    false);
        }
        return m_State.m_ModelTextures[m_State.m_CurrentDirectory / file_name.C_Str()];
    }
    void ModelLoader::ReadModelProperties(aiNode* node, const aiScene* scene) {
        ++m_State.m_NodesAmount;
        m_State.m_MeshesAmount += node->mNumMeshes;
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            m_State.m_VerticesAmount += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ReadModelProperties(node->mChildren[i], scene);
        }
    }
}  // namespace DE