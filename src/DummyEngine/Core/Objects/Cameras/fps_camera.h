#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Config/config.h"


namespace DE {
class FPSCamera {
private:
    float _fov = DEFAULT_CAMERA_FOV;
    float _aspect = DEFAULT_CAMERA_ASPECT;
    float _near_plane = DEFAULT_NEAR_PLANE;
    float _far_plane = DEFAULT_FAR_PLANE;

    glm::vec3 _world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _direction = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _right = glm::normalize(glm::cross(_position, _world_up));
    glm::vec3 _up = glm::normalize(glm::cross(_direction, _right));

    glm::mat4 _view_matrix;
    glm::mat4 _projection_matrix;

    void UpdateProjectionMatrix();
    void UpdateViewMatrix();

public:
    FPSCamera();
    FPSCamera(glm::vec3 camera_position);
    FPSCamera(glm::vec3 camera_position, glm::vec3 camera_direction);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::vec3 GetPos() const;
    glm::vec3 GetDir() const;
    float GetFov() const;
    float GetAspect() const;
    float GetNearPlane() const;
    float GetFarPlane() const;

    void LookAt(glm::vec3 point);
    void SetPos(glm::vec3 pos);
    void SetDir(glm::vec3 camera_direction);
    void SetFov(float fov);
    void SetAspect(float aspect);
    void SetNearPlane(float dist);
    void SetFarPlane(float dist);

    void RotateY(float angle);
    void RotateX(float angle);

    void MoveInWorld(const glm::vec3& delta);
    void MoveInLocal(const glm::vec3& delta);
};
}  // namespace DE