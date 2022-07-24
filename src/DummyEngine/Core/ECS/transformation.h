#pragma once

#include "DummyEngine/Addition/Base.h"

namespace DE {
class Transformation {
private:
    bool m_UpdateModel;
    bool m_UpdateRotation;

    Vec3 m_Scale;
    Vec3 m_Pos, m_PosOffset;
    Vec3 m_Angles, m_AnglesOffet;

    Mat4 m_ModelMatrix, m_RotationMatrix;

    void UpdateModelMatrix();
    void UpdateRotationMatrix();

public:
    Transformation();

    void MoveInWorld(Vec3 delta);
    void SetAnglesOffset(Vec3 angles);
    void SetAngles(Vec3 angles);
    void SetPosOffset(Vec3 offset);
    void SetPos(Vec3 pos);
    void SetPitch(float pitch);
    void SetYaw(float yaw);
    void SetRoll(float roll);
    void SetScale(float scale);
    void SetScale(Vec3 scale);

    Mat4 GetModelMatrix() const;
    Mat4 GetRotationMatrix() const;
    Vec3 GetPos() const;
    Vec3 GetPosOffset() const;
    Vec3 GetAngles() const;
    float GetPitch() const;
    float GetYaw() const;
    float GetRoll() const;
    Vec3 GetScale() const;

    void Update();
};
}  // namespace DE