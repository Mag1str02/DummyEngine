#include "Components.h"

namespace DummyEngine {

    Mat4 TransformComponent::GetTransform() const {
        return glm::translate(glm::mat4(1.0f), Translation + TranslationOffset) * glm::toMat4(glm::quat(glm::radians(Rotation + RotationOffet))) *
               glm::scale(glm::mat4(1.0f), Scale * ScaleOffset);
    }
    Mat4 TransformComponent::GetTranslation() const {
        return glm::translate(glm::mat4(1.0f), Translation + TranslationOffset);
    }
    Mat4 TransformComponent::GetRotation() const {
        return glm::toMat4(glm::quat(glm::radians(Rotation + RotationOffet)));
    }
    Mat4 TransformComponent::GetScale() const {
        return glm::scale(glm::mat4(1.0f), Scale * ScaleOffset);
    }

}  // namespace DummyEngine