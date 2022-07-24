#include "DummyEngine/Core/ECS/Transformation.h"

namespace DE {
void Transformation::UpdateModelMatrix() {
    m_ModelMatrix = Mat4(1.0f);
    m_ModelMatrix = glm::translate(m_ModelMatrix, m_Pos + m_PosOffset);
    m_ModelMatrix *= m_RotationMatrix;
    m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
    m_UpdateModel = false;
}
void Transformation::UpdateRotationMatrix() {
    m_RotationMatrix = Mat4(1.0f);
    m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Angles.z + m_AnglesOffet.z, Vec3(0.0f, 0.0f, 1.0f));
    m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Angles.y + m_AnglesOffet.y, Vec3(0.0f, 1.0f, 0.0f));
    m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Angles.x + m_AnglesOffet.x, Vec3(1.0f, 0.0f, 0.0f));
    UpdateModelMatrix();
    m_UpdateRotation = false;
}

Transformation::Transformation()
    : m_Scale(Vec3(1.0f)),                     //
      m_UpdateModel(true),                         //
      m_UpdateRotation(true),                      //
      m_Pos(Vec3(0.0f, 0.0f, 0.0f)),           //
      m_PosOffset(Vec3(0.0f, 0.0f, 0.0f)),    //
      m_Angles(Vec3(0.0f, 0.0f, 0.0f)),        //
      m_AnglesOffet(Vec3(0.0f, 0.0f, 0.0f))  //
{
}

void Transformation::MoveInWorld(Vec3 delta) {
    m_Pos += delta;
    m_UpdateModel = true;
}
void Transformation::SetScale(Vec3 scale) {
    m_Scale = scale;
    m_UpdateModel = true;
}
void Transformation::SetScale(float scale) {
    m_Scale = Vec3(scale);
    m_UpdateModel = true;
}
void Transformation::SetPosOffset(Vec3 offset) {
    m_PosOffset = offset;
    m_UpdateModel = true;
}
void Transformation::SetPos(Vec3 pos) {
    m_Pos = pos;
    m_UpdateModel = true;
}
void Transformation::SetPitch(float pitch) {
    m_Angles.x = pitch;
    m_UpdateRotation = true;
}
void Transformation::SetYaw(float yaw) {
    m_Angles.y = yaw;
    m_UpdateRotation = true;
}
void Transformation::SetRoll(float roll) {
    m_Angles.z = roll;
    m_UpdateRotation = true;
}
void Transformation::SetAngles(Vec3 angles) {
    m_Angles = angles;
    m_UpdateRotation = true;
}
void Transformation::SetAnglesOffset(Vec3 angles) {
    m_AnglesOffet = angles;
    m_UpdateRotation = true;
}

Mat4 Transformation::GetModelMatrix() const {
    return m_ModelMatrix;
}
Mat4 Transformation::GetRotationMatrix() const {
    return m_RotationMatrix;
}
Vec3 Transformation::GetPos() const {
    return m_Pos;
}
Vec3 Transformation::GetPosOffset() const {
    return m_PosOffset;
}
Vec3 Transformation::GetAngles() const {
    return m_Angles;
}
float Transformation::GetPitch() const {
    return m_Angles.x;
}
float Transformation::GetYaw() const {
    return m_Angles.y;
}
float Transformation::GetRoll() const {
    return m_Angles.z;
}
Vec3 Transformation::GetScale() const {
    return m_Scale;
}

void Transformation::Update() {
    if (m_UpdateRotation) {
        UpdateRotationMatrix();
    }
    if (m_UpdateModel) {
        UpdateModelMatrix();
    }
}
}  // namespace DE