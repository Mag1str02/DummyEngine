#include "Core/ECS/Components.h"

namespace DE
{
    Mat4 Transformation::GetTransform() const
    {
        return glm::translate(glm::mat4(1.0f), translation + translation_offset) *
               glm::toMat4(glm::quat(rotation + rotation_offet)) * glm::scale(glm::mat4(1.0f), scale + scale_offset);
    }
}  // namespace DE