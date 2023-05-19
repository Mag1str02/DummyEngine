#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    class SceneLoader;

    class FPSCamera {
        // TODO: Rework when render graph implemented
    public:
        float m_FOV       = Constants::DefaultCameraFOV;
        float m_Aspect    = Constants::DefaultCameraAspect;
        float m_NearPlane = Constants::DefaultCameraNearPlane;
        float m_FarPlane  = Constants::DefaultCameraFarPlane;

        Vec3 m_WorldUp  = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 m_Position = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 m_Direction;
        Vec3 m_Right;
        Vec3 m_Up;

        FPSCamera();
        FPSCamera(Vec3 camera_position);
        FPSCamera(Vec3 camera_position, Vec3 camera_direction);

        Mat4  GetViewMatrix() const;
        Mat4  GetProjectionMatrix() const;
        Mat4  GetViewProjection() const;
        Vec3  GetPos() const;
        Vec3  GetDir() const;
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