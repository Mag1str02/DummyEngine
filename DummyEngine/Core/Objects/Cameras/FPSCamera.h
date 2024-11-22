#pragma once

#include "DummyEngine/Utils/Types/Constants.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class FPSCamera {
        // TODO: Rework when render graph implemented
    public:
        float FOV       = Constants::kDefaultCameraFOV;
        float Aspect    = Constants::kDefaultCameraAspect;
        float NearPlane = Constants::kDefaultCameraNearPlane;
        float FarPlane  = Constants::kDefaultCameraFarPlane;

        Vec3 WorldUp  = Vec3(0.0f, 1.0f, 0.0f);
        Vec3 Position = Vec3(0.0f, 0.0f, 0.0f);
        Vec3 Direction;
        Vec3 Right;
        Vec3 Up;

        FPSCamera();
        FPSCamera(Vec3 camera_position, Vec3 camera_direction);
        explicit FPSCamera(Vec3 camera_position);

        Mat4  GetViewMatrix() const;
        Mat4  GetProjectionMatrix() const;
        Mat4  GetOrthoProjectionMatrix() const;
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

}  // namespace DummyEngine