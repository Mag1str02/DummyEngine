#pragma once

#include "DummyEngine/Core/Animations/Animation.h"

namespace DE {
    class Shader;
    class Animator {
    public:
        Animator(Ref<Animation> animation);
        void           SetTime(float tm);
        Ref<Animation> GetAnimation();

        void UpdateAnimation(float dt);
        void SetMatricies(Ref<Shader> shader, const std::string& name = "u_BoneMatrices");

    private:
        void CalculateBoneTransform(const Animation::Node* node, Mat4 parentTransform);

        std::vector<Mat4> m_FinalBoneMatrices;
        Ref<Animation>    m_CurrentAnimation;
        float             m_CurrentTime;
    };
}  // namespace DE