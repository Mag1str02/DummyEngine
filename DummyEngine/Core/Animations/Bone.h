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
        Bone(const std::string& name, U32 ID);

        Mat4        GetTransform(float animationTime);
        std::string GetBoneName() const;
        U32         GetBoneID() const;

    private:
        float GetScaleFactor(float prev_time, float next_time, float time);

        U32 GetPositionIndex(float time);
        U32 GetRotationIndex(float time);
        U32 GetScaleIndex(float time);

        Mat4 InterpolatePosition(float time);
        Mat4 InterpolateRotation(float time);
        Mat4 InterpolateScaling(float time);

    private:
        friend class ModelLoader;

        std::vector<KeyPosition> m_Positions;
        std::vector<KeyRotation> m_Rotations;
        std::vector<KeyScale>    m_Scales;

        std::string m_Name;
        U32         m_ID;
        Mat4        m_LocalTransform;
    };
    struct BoneInfo {
        Bone bone;
        Mat4 offset = Mat4(1.0);
    };
}  // namespace DE