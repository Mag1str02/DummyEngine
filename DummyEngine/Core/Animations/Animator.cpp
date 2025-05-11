#include "Animator.h"

#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

#include <format>

namespace DummyEngine {

    Animator::Animator(Ref<Animation> animation) : final_bone_matrices_(MAX_BONES, Mat4(1.0f)) {
        current_time_      = 0.0;
        current_animation_ = animation;
        CalculateBoneTransform(&current_animation_->GetRootNode(), Mat4(1.0f));
    }
    void Animator::SetTime(float tm) {
        auto next_time = fmod(tm, current_animation_->GetDuration());
        // printf("Setting time %.06f\n", tm);
        if (next_time != current_time_) {
            current_time_ = next_time;
            CalculateBoneTransform(&current_animation_->GetRootNode(), Mat4(1.0f));
        }
    }

    void Animator::UpdateAnimation(float dt) {
        SetTime(current_animation_->GetTicksPerSecond() * dt + current_time_);
    }

    void Animator::SetMatricies(Ref<Shader> shader, const std::string& name) {
        for (U32 i = 0; i < final_bone_matrices_.size(); ++i) {
            shader->SetMat4(std::format("{}[{}]", name, i), final_bone_matrices_[i]);
        }
    }
    Ref<Animation> Animator::GetAnimation() {
        return current_animation_;
    }

    void Animator::CalculateBoneTransform(const Animation::Node* node, Mat4 parent_transform) {
        Mat4      node_transform = node->Transformation;
        BoneInfo* bone_info      = current_animation_->GetBone(node->Name);
        if (bone_info != nullptr) {
            node_transform                                    = bone_info->Bone.GetTransform(current_time_);
            final_bone_matrices_[bone_info->Bone.GetBoneID()] = parent_transform * node_transform * bone_info->Offset;
        }
        Mat4 final_transform = parent_transform * node_transform;
        for (U32 i = 0; i < node->Childrens.size(); ++i) {
            CalculateBoneTransform(&node->Childrens[i], final_transform);
        }
    }

}  // namespace DummyEngine