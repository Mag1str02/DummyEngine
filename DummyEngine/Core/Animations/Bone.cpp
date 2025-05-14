#include "Bone.h"

namespace DummyEngine {

    Bone::Bone(const std::string& name, U32 id) : name_(name), id_(id) {}

    Mat4 Bone::GetTransform(float animation_time) const {
        Mat4 translation = InterpolatePosition(animation_time);
        Mat4 rotation    = InterpolateRotation(animation_time);
        Mat4 scale       = InterpolateScaling(animation_time);
        return translation * rotation * scale;
    }
    std::string Bone::GetBoneName() const {
        return name_;
    }
    U32 Bone::GetBoneID() const {
        return id_;
    }

    void Bone::AddPosition(KeyPosition position) {
        positions_.emplace_back(position);
    }
    void Bone::AddRotation(KeyRotation rotation) {
        rotations_.emplace_back(rotation);
    }
    void Bone::AddScale(KeyScale scale) {
        scales_.emplace_back(scale);
    }

    float Bone::GetScaleFactor(float prev_time, float next_time, float time) const {
        if ((next_time - prev_time) < 0.000001) {
            return 0;
        }
        float scale = (time - prev_time) / (next_time - prev_time);
        return scale;
    }

    U32 Bone::GetPositionIndex(float time) const {
        for (U32 i = 0; i < positions_.size() - 1; ++i) {
            if (time < positions_[i + 1].TimeStamp) {
                return i;
            }
        }
        return positions_.size() - 1;
    }
    U32 Bone::GetRotationIndex(float time) const {
        for (U32 i = 0; i < rotations_.size() - 1; ++i) {
            if (time < rotations_[i + 1].TimeStamp) {
                return i;
            }
        }
        return rotations_.size() - 1;
    }
    U32 Bone::GetScaleIndex(float time) const {
        for (U32 i = 0; i < scales_.size() - 1; ++i) {
            if (time < scales_[i + 1].TimeStamp) {
                return i;
            }
        }
        return scales_.size() - 1;
    }

    Mat4 Bone::InterpolatePosition(float time) const {
        if (positions_.empty()) {
            return Mat4(1.0);
        }
        U32 i = GetPositionIndex(time);
        if (i + 1 == positions_.size()) {
            return glm::translate(Mat4(1.0f), positions_.back().Position);
        }
        float     scale = GetScaleFactor(positions_[i].TimeStamp, positions_[i + 1].TimeStamp, time);
        glm::vec3 pos   = glm::mix(positions_[i].Position, positions_[i + 1].Position, scale);
        return glm::translate(Mat4(1.0f), pos);
    }
    Mat4 Bone::InterpolateRotation(float time) const {
        if (rotations_.empty()) {
            return Mat4(1.0);
        }
        U32 i = GetRotationIndex(time);
        if (i + 1 == rotations_.size()) {
            return glm::toMat4(glm::normalize(rotations_.back().Orientation));
        }
        float     scale    = GetScaleFactor(rotations_[i].TimeStamp, rotations_[i + 1].TimeStamp, time);
        glm::quat rotation = glm::slerp(rotations_[i].Orientation, rotations_[i + 1].Orientation, scale);
        return glm::toMat4(glm::normalize(rotation));
    }
    Mat4 Bone::InterpolateScaling(float time) const {
        if (scales_.empty()) {
            return Mat4(1.0);
        }
        U32 i = GetScaleIndex(time);
        if (i + 1 == scales_.size()) {
            return glm::scale(Mat4(1.0f), scales_.back().Scale);
        }
        float     scale   = GetScaleFactor(scales_[i].TimeStamp, scales_[i + 1].TimeStamp, time);
        glm::vec3 scaling = glm::mix(scales_[i].Scale, scales_[i + 1].Scale, scale);
        return glm::scale(Mat4(1.0f), scaling);
    }

}  // namespace DummyEngine

/*
 */