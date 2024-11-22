#pragma once

#include "DummyEngine/Core/Animations/Animation.h"

namespace DummyEngine {

    class Shader;
    class Animator {
    public:
        explicit Animator(Ref<Animation> animation);

        void           SetTime(float tm);
        Ref<Animation> GetAnimation();

        void UpdateAnimation(float dt);
        void SetMatricies(Ref<Shader> shader, const std::string& name = "u_BoneMatrices");

    private:
        void CalculateBoneTransform(const Animation::Node* node, Mat4 parent_transform);

        std::vector<Mat4> final_bone_matrices_;
        Ref<Animation>    current_animation_;
        float             current_time_;
    };

}  // namespace DummyEngine