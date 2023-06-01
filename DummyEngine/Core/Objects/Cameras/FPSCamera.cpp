#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

namespace DE {

    FPSCamera::FPSCamera() {
        SetDir(Vec3(0.0f, 0.0f, -1.0f));
    }
    FPSCamera::FPSCamera(Vec3 camera_position) : FPSCamera() {
        m_Position = camera_position;
    }
    FPSCamera::FPSCamera(Vec3 camera_position, Vec3 camera_direction) : FPSCamera(camera_position) {
        m_Direction = glm::normalize(camera_direction);
        m_Right     = glm::normalize(glm::cross(m_Direction, m_WorldUp));
        m_Up        = glm::normalize(glm::cross(m_Right, m_Direction));
    }

    void FPSCamera::MoveInWorld(const Vec3& delta) {
        m_Position += delta;
    }
    void FPSCamera::MoveInLocal(const Vec3& delta) {
        m_Position += delta.x * m_Right;
        m_Position += delta.y * m_Up;
        m_Position += delta.z * m_Direction;
    }

    void FPSCamera::RotateY(float angle) {
        SetDir(glm::rotate(m_Direction, glm::radians(-angle), m_WorldUp));
    }
    void FPSCamera::RotateX(float angle) {
        angle *= -1;
        float critical_angle = 89.0f;
        float h_angle        = glm::angle(m_Direction, m_WorldUp) * 180 / M_PI;
        h_angle              = -90.0f + (180 - h_angle);
        float new_h_angle    = angle + h_angle;
        if (new_h_angle > critical_angle) new_h_angle = critical_angle;
        if (new_h_angle < -critical_angle) new_h_angle = -critical_angle;
        SetDir(glm::rotate(m_Direction, glm::radians(new_h_angle - h_angle), m_Right));
    }

    Mat4 FPSCamera::GetProjectionMatrix() const {
        Mat4 projection = Mat4(1.0f);
        projection      = glm::perspective(glm::radians(m_FOV), m_Aspect, m_NearPlane, m_FarPlane);
        return projection;
    }
    Mat4 FPSCamera::GetViewMatrix() const {
        Mat4 view  = Mat4(1.0f);
        Mat4 buff  = Mat4(1.0f);
        buff[0][0] = m_Right.x;
        buff[1][0] = m_Right.y;
        buff[2][0] = m_Right.z;
        buff[0][1] = m_Up.x;
        buff[1][1] = m_Up.y;
        buff[2][1] = m_Up.z;
        buff[0][2] = -m_Direction.x;
        buff[1][2] = -m_Direction.y;
        buff[2][2] = -m_Direction.z;

        view = view * buff;
        view = glm::translate(view, -m_Position);
        return view;
    }
    Mat4 FPSCamera::GetViewProjection() const {
        return GetProjectionMatrix() * GetViewMatrix();
    }

    Vec3 FPSCamera::GetPos() const {
        return m_Position;
    }
    Vec3 FPSCamera::GetDir() const {
        return m_Direction;
    }
    float FPSCamera::GetFov() const {
        return m_FOV;
    }
    float FPSCamera::GetAspect() const {
        return m_Aspect;
    }
    float FPSCamera::GetNearPlane() const {
        return m_NearPlane;
    }
    float FPSCamera::GetFarPlane() const {
        return m_FarPlane;
    }

    void FPSCamera::LookAt(Vec3 point) {
        m_Direction = glm::normalize(point - m_Position);
    }
    void FPSCamera::SetDir(Vec3 camera_direction) {
        m_Direction = glm::normalize(camera_direction);
        m_Right     = glm::normalize(glm::cross(m_Direction, m_WorldUp));
        m_Up        = glm::normalize(glm::cross(m_Right, m_Direction));
    }
    void FPSCamera::SetPos(Vec3 pos) {
        m_Position = pos;
    }
    void FPSCamera::SetFov(float fov) {
        m_FOV = fov;
        if (m_FOV > 179.0f) {
            m_FOV = 179.0f;
        }
        if (m_FOV < 1.0f) {
            m_FOV = 1.0f;
        }
    }
    void FPSCamera::SetAspect(float aspect) {
        m_Aspect = aspect;
    }
    void FPSCamera::SetNearPlane(float dist) {
        m_NearPlane = dist;
    }
    void FPSCamera::SetFarPlane(float dist) {
        m_FarPlane = dist;
    }
}  // namespace DE