#pragma once

#include "Addition/Base.h"

namespace DE
{
    class FPSCamera
    {
    private:
        float m_FOV = Config::GetF(DE_CFG_DEFAULT_CAMERA_FOV);
        float m_Aspect = Config::GetF(DE_CFG_DEFAULT_CAMERA_ASPECT);
        float m_NearPlane = Config::GetF(DE_CFG_DEFAULT_CAMERA_NEAR_PLANE);
        float m_FarPlane = Config::GetF(DE_CFG_DEFAULT_CAMERA_FAR_PLANE);

        Vec3 m_WorldUp = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 m_Position = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 m_Direction = Vec3(0.0f, 0.0f, -1.0f);
        Vec3 m_Right = glm::normalize(glm::cross(m_Position, m_WorldUp));
        Vec3 m_Up = glm::normalize(glm::cross(m_Direction, m_Right));

        Mat4 m_ViewMatrix;
        Mat4 m_ProjectionMatrix;

        void UpdateProjectionMatrix();
        void UpdateViewMatrix();

    public:
        FPSCamera();
        FPSCamera(Vec3 camera_position);
        FPSCamera(Vec3 camera_position, Vec3 camera_direction);

        Mat4 GetViewMatrix() const;
        Mat4 GetProjectionMatrix() const;
        Mat4 GetViewProjection() const;
        Vec3 GetPos() const;
        Vec3 GetDir() const;
        float GetFov() const;
        float GetAspect() const;
        float GetNearPlane() const;
        float GetFarPlane() const;

        void LookAt(Vec3 point);
        void SetPos(Vec3 pos);
        void SetDir(Vec3 camera_direction);
        void SetFov(float fov);
        void SetAspect(float aspect);
        void SetNearPlane(float dist);
        void SetFarPlane(float dist);

        void RotateY(float angle);
        void RotateX(float angle);

        void MoveInWorld(const Vec3& delta);
        void MoveInLocal(const Vec3& delta);
    };
}  // namespace DE