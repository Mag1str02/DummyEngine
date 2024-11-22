#include "FPSCamera.h"

#include <glm/gtx/vector_angle.hpp>  // IWYU pragma: export

namespace DummyEngine {

    FPSCamera::FPSCamera() {
        SetDir(Vec3(0.0f, 0.0f, -1.0f));
    }
    FPSCamera::FPSCamera(Vec3 camera_position) : FPSCamera() {
        Position = camera_position;
    }
    FPSCamera::FPSCamera(Vec3 camera_position, Vec3 camera_direction) : FPSCamera(camera_position) {
        Direction = glm::normalize(camera_direction);
        Right     = glm::normalize(glm::cross(Direction, WorldUp));
        Up        = glm::normalize(glm::cross(Right, Direction));
        FarPlane  = 100;
        NearPlane = -10;
    }

    void FPSCamera::MoveInWorld(const Vec3& delta) {
        Position += delta;
    }
    void FPSCamera::MoveInLocal(const Vec3& delta) {
        Position += delta.x * Right;
        Position += delta.y * Up;
        Position += delta.z * Direction;
    }

    void FPSCamera::RotateY(float angle) {
        SetDir(glm::rotate(Direction, glm::radians(-angle), WorldUp));
    }
    void FPSCamera::RotateX(float angle) {
        angle *= -1;
        float critical_angle = 89.0f;
        float h_angle        = glm::angle(Direction, WorldUp) * 180 / M_PI;
        h_angle              = -90.0f + (180 - h_angle);
        float new_h_angle    = angle + h_angle;
        if (new_h_angle > critical_angle) {
            new_h_angle = critical_angle;
        }
        if (new_h_angle < -critical_angle) {
            new_h_angle = -critical_angle;
        }
        SetDir(glm::rotate(Direction, glm::radians(new_h_angle - h_angle), Right));
    }

    Mat4 FPSCamera::GetProjectionMatrix() const {
        Mat4 projection = Mat4(1.0f);
        projection      = glm::perspective(glm::radians(FOV), Aspect, NearPlane, FarPlane);
        return projection;
    }

    Mat4 FPSCamera::GetOrthoProjectionMatrix() const {
        Mat4 projection = Mat4(1.0f);
        projection      = glm::ortho(-100.f, 100.f, -100.f, 100.f, NearPlane, FarPlane);
        return projection;
    }
    Mat4 FPSCamera::GetViewMatrix() const {
        Mat4 view  = Mat4(1.0f);
        Mat4 buff  = Mat4(1.0f);
        buff[0][0] = Right.x;
        buff[1][0] = Right.y;
        buff[2][0] = Right.z;
        buff[0][1] = Up.x;
        buff[1][1] = Up.y;
        buff[2][1] = Up.z;
        buff[0][2] = -Direction.x;
        buff[1][2] = -Direction.y;
        buff[2][2] = -Direction.z;

        view = view * buff;
        view = glm::translate(view, -Position);
        return view;
    }
    Mat4 FPSCamera::GetViewProjection() const {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    Vec3 FPSCamera::GetPos() const {
        return Position;
    }
    Vec3 FPSCamera::GetDir() const {
        return Direction;
    }
    float FPSCamera::GetFov() const {
        return FOV;
    }
    float FPSCamera::GetAspect() const {
        return Aspect;
    }
    float FPSCamera::GetNearPlane() const {
        return NearPlane;
    }
    float FPSCamera::GetFarPlane() const {
        return FarPlane;
    }

    void FPSCamera::LookAt(Vec3 point) {
        Direction = glm::normalize(point - Position);
    }
    void FPSCamera::SetDir(Vec3 camera_direction) {
        Direction = glm::normalize(camera_direction);
        Right     = glm::normalize(glm::cross(Direction, WorldUp));
        Up        = glm::normalize(glm::cross(Right, Direction));
    }
    void FPSCamera::SetPos(Vec3 pos) {
        Position = pos;
    }
    void FPSCamera::SetFov(float fov) {
        FOV = fov;
        if (FOV > 179.0f) {
            FOV = 179.0f;
        }
        if (FOV < 1.0f) {
            FOV = 1.0f;
        }
    }
    void FPSCamera::SetAspect(float aspect) {
        Aspect = aspect;
    }
    void FPSCamera::SetNearPlane(float dist) {
        NearPlane = dist;
    }
    void FPSCamera::SetFarPlane(float dist) {
        FarPlane = dist;
    }
}  // namespace DummyEngine