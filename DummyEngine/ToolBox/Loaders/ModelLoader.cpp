#include "ModelLoader.h"

#include "DummyEngine/Core/Application/Concurrency.h"
#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/Utils/Debug/Logger.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace DummyEngine {

    class ModelLoaderImpl {
    private:
        LOG_AUTHOR(ModelLoader)

        enum ColorType {
            None = 0,
            Albedo,
            Diffuse,
            Specular,
            Ambient,
            ORM,
            Emission,
        };

    public:
        explicit ModelLoaderImpl(const RenderMeshAsset::LoadingProperties& properties) : props_(properties) {}

        Result<Ref<RenderMeshData>> Load();

    private:
        void LoadBone(Bone& bone, aiNodeAnim* node);
        void ReadWeights(aiMesh* mesh);
        void ReadBones(Animation& animation, const aiAnimation* anim);
        void ReadAnimationNode(Animation::Node& node, const aiNode* src);
        void ReadAnimation(Animation& animation, const aiScene* scene);
        void FinalizeMaterials();

        TryFuture<MaterialData>     LoadMaterial(aiMaterial* mat);
        TryFuture<Ref<TextureData>> GetTexture(aiMaterial* mat, aiTextureType type);

        void ProcessNode(aiNode* node, const aiScene* scene);
        void ProcessMesh(aiMesh* mesh, const aiScene* scene);
        Vec3 GetColor(aiMaterial* mat, ColorType type);
        void ReadModelProperties(aiNode* node, const aiScene* scene);

    private:
        size_t                             vertices_amount_;
        size_t                             nodes_amount_;
        size_t                             current_mesh_id_;
        size_t                             meshes_amount_;
        Path                               current_directory_;
        RenderMeshAsset::LoadingProperties props_;

        Ref<RenderMeshData>                                       current_data_;
        std::vector<TryFuture<MaterialData>>                      meshes_material_;
        std::unordered_map<Path, CopyTryFuture<Ref<TextureData>>> model_textures_;
        Assimp::Importer                                          importer_;
    };

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

    void ModelLoaderImpl::LoadBone(Bone& bone, aiNodeAnim* node) {
        for (U32 i = 0; i < node->mNumPositionKeys; ++i) {
            auto        pos        = node->mPositionKeys[i].mValue;
            float       time_stamp = node->mPositionKeys[i].mTime;
            KeyPosition data;
            data.Position.x = pos.x;
            data.Position.y = pos.y;
            data.Position.z = pos.z;
            data.TimeStamp  = time_stamp;
            bone.AddPosition(data);
        }

        for (U32 i = 0; i < node->mNumRotationKeys; ++i) {
            auto        rot        = node->mRotationKeys[i].mValue;
            float       time_stamp = node->mRotationKeys[i].mTime;
            KeyRotation data;
            data.Orientation.x = rot.x;
            data.Orientation.y = rot.y;
            data.Orientation.z = rot.z;
            data.Orientation.w = rot.w;
            data.TimeStamp     = time_stamp;
            bone.AddRotation(data);
        }

        for (U32 i = 0; i < node->mNumScalingKeys; ++i) {
            aiVector3D scale      = node->mScalingKeys[i].mValue;
            float      time_stamp = node->mScalingKeys[i].mTime;
            KeyScale   data;
            data.Scale.x   = scale.x;
            data.Scale.y   = scale.y;
            data.Scale.z   = scale.z;
            data.TimeStamp = time_stamp;
            bone.AddScale(data);
        }
        // LOG_INFO("Loaded bone ({}|{}), with positions ({}), rotations ({}) and scales ({})",
        //          bone.GetBoneName(),
        //          bone.GetBoneID(),
        //          bone.m_Positions.size(),
        //          bone.m_Rotations.size(),
        //          bone.m_Scales.size());
    }

    void ModelLoaderImpl::ReadWeights(aiMesh* mesh) {
        auto& model        = *current_data_;
        auto& current_mesh = model.Meshes[current_mesh_id_];
        if (!model.Animation) {
            return;
        }
        auto& animation = *model.Animation;

        for (U32 i = 0; i < mesh->mNumBones; ++i) {
            int         bone_id   = -1;
            std::string bone_name = mesh->mBones[i]->mName.C_Str();
            if (!animation.bone_name_to_id_.contains(bone_name)) {
                bone_id                               = animation.bones_.size();
                animation.bone_name_to_id_[bone_name] = bone_id;

                BoneInfo bone_info;
                bone_info.Bone   = Bone(bone_name, bone_id);
                bone_info.Offset = AssimpToGLM(mesh->mBones[i]->mOffsetMatrix);
                animation.bones_.push_back(bone_info);
                // printf("Added bone (%s) with id (%d)\n", bone_name.c_str(), bone_id);
            } else {
                bone_id = animation.bone_name_to_id_[bone_name];
            }
            if (bone_id == -1) {
                LOG_WARNING("Wrong bone id");
                return;
            }
            auto weights        = mesh->mBones[i]->mWeights;
            int  weights_amount = mesh->mBones[i]->mNumWeights;

            for (int j = 0; j < weights_amount; ++j) {
                U32   v_id   = weights[j].mVertexId;
                float weight = weights[j].mWeight;

                if (v_id >= current_mesh.Vertices.size()) {
                    LOG_WARNING("Wrong vertex id");
                    return;
                }
                current_mesh.Vertices[v_id].AddBone(bone_id, weight);
            }
        }
    }
    void ModelLoaderImpl::ReadBones(Animation& animation, const aiAnimation* anim) {
        for (U32 i = 0; i < anim->mNumChannels; ++i) {
            auto channel   = anim->mChannels[i];
            auto bone_info = animation.GetBone(channel->mNodeName.data);
            if (bone_info == nullptr) {
                BoneInfo info;
                info.Offset                                         = glm::mat4(1.0);
                animation.bone_name_to_id_[channel->mNodeName.data] = animation.bones_.size();
                animation.bones_.push_back(info);
                bone_info = &animation.bones_.back();
                LOG_WARNING("Added unknown bone");
            }
            LoadBone(bone_info->Bone, channel);
        }
    }
    void ModelLoaderImpl::ReadAnimationNode(Animation::Node& node, const aiNode* src) {
        node.Name           = src->mName.data;
        node.Transformation = AssimpToGLM(src->mTransformation);
        node.Childrens.resize(src->mNumChildren);

        for (U32 i = 0; i < src->mNumChildren; i++) {
            Animation::Node& data = node.Childrens[i];
            ReadAnimationNode(data, src->mChildren[i]);
        }
    }
    void ModelLoaderImpl::ReadAnimation(Animation& animation, const aiScene* scene) {
        auto anim                   = scene->mAnimations[0];
        animation.duration_         = anim->mDuration;
        animation.ticks_per_second_ = anim->mTicksPerSecond;
        ReadAnimationNode(animation.root_node_, scene->mRootNode);
        ReadBones(animation, anim);
    }

    TryFuture<MaterialData> ModelLoaderImpl::LoadMaterial(aiMaterial* mat) {
        return Futures::Submit(Concurrency::GetEngineBackgroundScheduler(), [this, mat]() -> Result<MaterialData> {
            try {
                MaterialData material;
                material.Diffuse  = GetColor(mat, ColorType::Diffuse);
                material.Specular = GetColor(mat, ColorType::Specular);
                material.Ambient  = GetColor(mat, ColorType::Ambient);
                material.Albedo   = GetColor(mat, ColorType::Albedo);
                material.ORM      = GetColor(mat, ColorType::ORM);
                material.Emission = GetColor(mat, ColorType::Emission);
                aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &material.Shininess);

                auto albedo   = GetTexture(mat, aiTextureType_DIFFUSE);
                auto normal   = GetTexture(mat, aiTextureType_NORMALS);
                auto orm      = GetTexture(mat, aiTextureType_METALNESS);
                auto diffuse  = GetTexture(mat, aiTextureType_DIFFUSE);
                auto specular = GetTexture(mat, aiTextureType_SPECULAR);
                auto emissive = GetTexture(mat, aiTextureType_EMISSIVE);

                material.AlbedoMap   = std::move(albedo) | Futures::GetOk();
                material.NormalMap   = std::move(normal) | Futures::GetOk();
                material.ORMMap      = std::move(orm) | Futures::GetOk();
                material.DiffuseMap  = std::move(diffuse) | Futures::GetOk();
                material.SpecularMap = std::move(specular) | Futures::GetOk();
                material.EmissionMap = std::move(emissive) | Futures::GetOk();

                return material;
            } catch (const std::exception& ex) {
                return Results::Failure();
            }
        });
    }

    Result<Ref<RenderMeshData>> ModelLoaderImpl::Load() {
        DE_PROFILE_SCOPE("ModelLoader::Load");
        unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
        if (props_.FlipUV) {
            flags |= aiProcess_FlipUVs;
        }
        const aiScene* scene = importer_.ReadFile(props_.Path.string(), flags);

        if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 || scene->mRootNode == nullptr) {
            LOG_ERROR("Failed to load model {} due: {}", Config::RelativeToExecutable(props_.Path), importer_.GetErrorString());
            return Results::Failure();
        }

        current_data_ = CreateRef<RenderMeshData>();
        if (scene->mNumAnimations > 0) {
            current_data_->Animation = CreateRef<Animation>();
        }
        current_mesh_id_   = 0;
        meshes_amount_     = 0;
        nodes_amount_      = 0;
        vertices_amount_   = 0;
        current_directory_ = props_.Path.parent_path();

        ReadModelProperties(scene->mRootNode, scene);
        LOG_INFO("Model {} loaded with {} meshes and {} verticies", Config::RelativeToExecutable(props_.Path), meshes_amount_, vertices_amount_);

        current_data_->Meshes.resize(meshes_amount_);
        meshes_material_.reserve(meshes_amount_);
        ProcessNode(scene->mRootNode, scene);
        if (current_data_->Animation) {
            ReadAnimation(*current_data_->Animation, scene);
        }

        for (U32 i = 0; i < meshes_material_.size(); ++i) {
            auto result = std::move(meshes_material_[i]) | Futures::Get();
            if (!result.has_value()) {
                LOG_ERROR("Failed to load material {} of model {} due: {}", i, Config::RelativeToExecutable(props_.Path), importer_.GetErrorString());
                return Results::Failure();
            }
            current_data_->Meshes[i].Material = std::move(result.value());
        }

        if (props_.Compress) {
            current_data_->Compress();
        }
        return current_data_;
    }

    void ModelLoaderImpl::ProcessNode(aiNode* node, const aiScene* scene) {
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene);
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    void ModelLoaderImpl::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        DE_PROFILE_SCOPE("ModelLoader::ProcessMesh");
        RenderMeshData&    model        = *current_data_;
        RenderSubMeshData& current_mesh = model.Meshes[current_mesh_id_];
        for (size_t i = 0; i < mesh->mNumVertices; ++i) {
            Vertex3D vertex;

            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            if (mesh->mNormals != nullptr) {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }
            if (mesh->mTangents != nullptr) {
                vertex.Tangent.x = mesh->mTangents[i].x;
                vertex.Tangent.y = mesh->mTangents[i].y;
                vertex.Tangent.z = mesh->mTangents[i].z;
            }
            if (mesh->mTextureCoords[0] != nullptr) {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }
            current_mesh.Vertices.push_back(vertex);
        }
        ReadWeights(mesh);
        for (size_t i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j) {
                current_mesh.Indices.push_back(face.mIndices[j]);
            }
        }
        // if (mesh->mMaterialIndex >= 0) {    // always true because of unsigned int  0 <= attachment_id
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        meshes_material_.emplace_back(LoadMaterial(material));
        ++current_mesh_id_;
    }
    Vec3 ModelLoaderImpl::GetColor(aiMaterial* mat, ColorType type) {
        DE_PROFILE_SCOPE("ModelLoader::GetColor");
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
    TryFuture<Ref<TextureData>> ModelLoaderImpl::GetTexture(aiMaterial* mat, aiTextureType type) {
        DE_PROFILE_SCOPE("ModelLoader::GetTexture");
        aiString file_name;
        Path     texture_path;
        if (mat->GetTextureCount(type) == 0) {
            return Futures::Ok(Ref<TextureData>());
        } else if (mat->GetTextureCount(type) > 1) {
            LOG_WARNING("Model has more multiple textures of same type. Loading only first one.");
        }
        mat->GetTexture(type, 0, &file_name);

        auto path = current_directory_ / file_name.C_Str();
        if (!model_textures_.contains(path)) {
            model_textures_[path] = TextureLoader::Load({path, false}) | Futures::Copy();
        }
        return model_textures_[path];
    }
    void ModelLoaderImpl::ReadModelProperties(aiNode* node, const aiScene* scene) {
        DE_PROFILE_SCOPE("ModelLoader::ReadModelProperties");
        ++nodes_amount_;
        meshes_amount_ += node->mNumMeshes;
        for (size_t i = 0; i < node->mNumMeshes; ++i) {
            vertices_amount_ += scene->mMeshes[node->mMeshes[i]]->mNumVertices;
        }
        for (size_t i = 0; i < node->mNumChildren; ++i) {
            ReadModelProperties(node->mChildren[i], scene);
        }
    }

    TryFuture<Ref<RenderMeshData>> ModelLoader::Load(const RenderMeshAsset::LoadingProperties& props) {
        return Futures::Submit(                           //
            Concurrency::GetEngineBackgroundScheduler(),  //
            [props]() {
                ModelLoaderImpl loader(props);
                return loader.Load();  //
            });
    }
}  // namespace DummyEngine