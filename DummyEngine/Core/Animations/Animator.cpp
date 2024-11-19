#include "DummyEngine/Core/Animations/Animator.h"

#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DE {
    Animator::Animator(Ref<Animation> animation) : m_FinalBoneMatrices(MAX_BONES, Mat4(1.0f)) {
        m_CurrentTime      = 0.0;
        m_CurrentAnimation = animation;
        CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), Mat4(1.0f));
    }
    void Animator::SetTime(float tm) {
        auto next_time = fmod(tm, m_CurrentAnimation->GetDuration());
        // printf("Setting time %.06f\n", tm);
        if (next_time != m_CurrentTime) {
            m_CurrentTime = next_time;
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), Mat4(1.0f));
        }
    }

    void Animator::UpdateAnimation(float dt) {
        SetTime(m_CurrentAnimation->GetTicksPerSecond() * dt + m_CurrentTime);
    }

    void Animator::SetMatricies(Ref<Shader> shader, const std::string& name) {
        for (U32 i = 0; i < m_FinalBoneMatrices.size(); ++i) {
            shader->SetMat4(std::format("{}[{}]", name, i), m_FinalBoneMatrices[i]);
        }
    }
    Ref<Animation> Animator::GetAnimation() {
        return m_CurrentAnimation;
    }

    void Animator::CalculateBoneTransform(const Animation::Node* node, Mat4 parent_transform) {
        Mat4      node_transform = node->transformation;
        BoneInfo* bone_info      = m_CurrentAnimation->GetBone(node->name);
        if (bone_info) {
            node_transform                                   = bone_info->bone.GetTransform(m_CurrentTime);
            m_FinalBoneMatrices[bone_info->bone.GetBoneID()] = parent_transform * node_transform * bone_info->offset;
        }
        Mat4 final_transform = parent_transform * node_transform;
        for (U32 i = 0; i < node->childrens.size(); ++i) {
            CalculateBoneTransform(&node->childrens[i], final_transform);
        }
    }
}  // namespace DE