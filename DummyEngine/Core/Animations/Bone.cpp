#include "DummyEngine/Core/Animations/Bone.h"

namespace DE {
    Bone::Bone(const std::string& name, S32 ID) : m_Name(name), m_ID(ID), m_LocalTransform(1.0f) {}

    Mat4 Bone::GetTransform(float animationTime) {
        Mat4 translation = InterpolatePosition(animationTime);
        Mat4 rotation    = InterpolateRotation(animationTime);
        Mat4 scale       = InterpolateScaling(animationTime);
        return translation * rotation * scale;
    }
    std::string Bone::GetBoneName() const {
        return m_Name;
    }
    S32 Bone::GetBoneID() const {
        return m_ID;
    }

    float Bone::GetScaleFactor(float prev_time, float next_time, float time) {
        if ((next_time - prev_time) < 0.000001) {
            return 0;
        }
        float scale = (time - prev_time) / (next_time - prev_time);
        return scale;
    }

    S32 Bone::GetPositionIndex(float time) {
        for (S32 i = 0; i < m_Positions.size() - 1; ++i) {
            if (time < m_Positions[i + 1].time_stamp) {
                return i;
            }
        }
        return m_Positions.size() - 1;
    }
    S32 Bone::GetRotationIndex(float time) {
        for (S32 i = 0; i < m_Rotations.size() - 1; ++i) {
            if (time < m_Rotations[i + 1].time_stamp) {
                return i;
            }
        }
        return m_Rotations.size() - 1;
    }
    S32 Bone::GetScaleIndex(float time) {
        for (S32 i = 0; i < m_Scales.size() - 1; ++i) {
            if (time < m_Scales[i + 1].time_stamp) {
                return i;
            }
        }
        return m_Scales.size() - 1;
    }

    Mat4 Bone::InterpolatePosition(float time) {
        if (m_Positions.empty()) {
            return Mat4(1.0);
        }
        S32 i = GetPositionIndex(time);
        // printf("pos %d\n", i);
        if (i == m_Positions.size() - 1) {
            return glm::translate(Mat4(1.0f), m_Positions.back().position);
        }
        float scale = GetScaleFactor(m_Positions[i].time_stamp, m_Positions[i + 1].time_stamp, time);
        // printf("pos %d %.06f %.06f %.06f\n", i, scale, m_Positions[i].time_stamp, m_Positions[i + 1].time_stamp);
        glm::vec3 pos = glm::mix(m_Positions[i].position, m_Positions[i + 1].position, scale);
        return glm::translate(Mat4(1.0f), pos);
    }
    Mat4 Bone::InterpolateRotation(float time) {
        if (m_Rotations.empty()) {
            return Mat4(1.0);
        }
        S32 i = GetRotationIndex(time);
        if (i == m_Rotations.size() - 1) {
            return glm::toMat4(glm::normalize(m_Rotations.back().orientation));
        }
        float     scale    = GetScaleFactor(m_Rotations[i].time_stamp, m_Rotations[i + 1].time_stamp, time);
        glm::quat rotation = glm::slerp(m_Rotations[i].orientation, m_Rotations[i + 1].orientation, scale);
        // printf("rot %d %.06f %.06f %.06f\n", i, scale, m_Rotations[i].time_stamp, m_Rotations[i + 1].time_stamp);
        return glm::toMat4(glm::normalize(rotation));
    }
    Mat4 Bone::InterpolateScaling(float time) {
        if (m_Scales.empty()) {
            return Mat4(1.0);
        }
        S32 i = GetScaleIndex(time);
        // printf("scale %d\n", i);
        if (i == m_Scales.size() - 1) {
            return glm::scale(Mat4(1.0f), m_Scales.back().scale);
        }
        float     scale   = GetScaleFactor(m_Scales[i].time_stamp, m_Scales[i + 1].time_stamp, time);
        glm::vec3 scaling = glm::mix(m_Scales[i].scale, m_Scales[i + 1].scale, scale);
        // printf("scale %d %.06f %.06f %.06f\n", i, scale, m_Scales[i].time_stamp, m_Scales[i + 1].time_stamp);
        return glm::scale(Mat4(1.0f), scaling);
    }

}  // namespace DE

/*
 */