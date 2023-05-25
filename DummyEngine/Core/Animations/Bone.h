#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    struct KeyPosition {
        Vec3  position;
        float time_stamp;
    };

    struct KeyRotation {
        Quaternion orientation;
        float      time_stamp;
    };

    struct KeyScale {
        Vec3  scale;
        float time_stamp;
    };

    class Bone {
    public:
        Bone() = default;
        Bone(const std::string& name, int ID);

        Mat4        GetTransform(float animationTime);
        std::string GetBoneName() const;
        S32         GetBoneID() const;

    private:
        float GetScaleFactor(float prev_time, float next_time, float time);

        S32 GetPositionIndex(float time);
        S32 GetRotationIndex(float time);
        S32 GetScaleIndex(float time);

        Mat4 InterpolatePosition(float time);
        Mat4 InterpolateRotation(float time);
        Mat4 InterpolateScaling(float time);

    private:
        friend class ModelLoader;

        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale>    m_Scales;

        S32         m_ID;
        Mat4        m_LocalTransform;
        std::string m_Name;
    };
    struct BoneInfo {
        Bone bone;
        Mat4 offset = Mat4(1.0);
    };
}  // namespace DE