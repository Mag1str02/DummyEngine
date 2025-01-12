#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    struct KeyPosition {
        Vec3  Position;
        float TimeStamp;
    };

    struct KeyRotation {
        Quaternion Orientation;
        float      TimeStamp;
    };

    struct KeyScale {
        Vec3  Scale;
        float TimeStamp;
    };

    class Bone {
    public:
        Bone() = default;
        Bone(const std::string& name, U32 id);

        Mat4        GetTransform(float animation_time);
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

        std::vector<KeyPosition> positions_;
        std::vector<KeyRotation> rotations_;
        std::vector<KeyScale>    scales_;

        std::string name_;
        U32         id_;
    };
    struct BoneInfo {
        Bone Bone;
        Mat4 Offset = Mat4(1.0);
    };

}  // namespace DummyEngine