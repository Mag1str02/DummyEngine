#include "fps_camera.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../../../UnSorted/Config/config.h"

FPSCamera::FPSCamera() {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}
FPSCamera::FPSCamera(glm::vec3 camera_position) : FPSCamera() {
    _position = camera_position;
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}
FPSCamera::FPSCamera(glm::vec3 camera_position, glm::vec3 camera_direction) : FPSCamera(camera_position) {
    _direction = glm::normalize(camera_direction);
    _right = glm::normalize(glm::cross(_direction, _world_up));
    _up = glm::normalize(glm::cross(_right, _direction));
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void FPSCamera::MoveInWorld(const glm::vec3& delta) {
    _position += delta;
    UpdateViewMatrix();
}
void FPSCamera::MoveInLocal(const glm::vec3& delta) {
    _position += delta.x * _right;
    _position += delta.y * _up;
    _position += delta.z * _direction;
    UpdateViewMatrix();
}

void FPSCamera::RotateY(float angle) {
    SetDir(glm::rotate(_direction, glm::radians(-angle), _world_up));
}
void FPSCamera::RotateX(float angle) {
    angle *= -1;
    float critical_angle = 89.0f;
    float h_angle = glm::angle(_direction, _world_up) * 180 / M_PI;
    h_angle = -90.0f + (180 - h_angle);
    float new_h_angle = angle + h_angle;
    if (new_h_angle > critical_angle)
        new_h_angle = critical_angle;
    if (new_h_angle < -critical_angle)
        new_h_angle = -critical_angle;
    SetDir(glm::rotate(_direction, glm::radians(new_h_angle - h_angle), _right));
}

glm::mat4 FPSCamera::GetProjectionMatrix() const {
    return _projection_matrix;
}
glm::mat4 FPSCamera::GetViewMatrix() const {
    return _view_matrix;
}
glm::vec3 FPSCamera::GetPos() const {
    return _position;
}
glm::vec3 FPSCamera::GetDir() const {
    return _direction;
}
float FPSCamera::GetFov() const {
    return _fov;
}
float FPSCamera::GetAspect() const {
    return _aspect;
}
float FPSCamera::GetNearPlane() const {
    return _near_plane;
}
float FPSCamera::GetFarPlane() const {
    return _far_plane;
}

void FPSCamera::LookAt(glm::vec3 point) {
    _direction = glm::normalize(point - _position);
    UpdateViewMatrix();
}
void FPSCamera::SetDir(glm::vec3 camera_direction) {
    _direction = glm::normalize(camera_direction);
    _right = glm::normalize(glm::cross(_direction, _world_up));
    _up = glm::normalize(glm::cross(_right, _direction));
    UpdateViewMatrix();
}
void FPSCamera::SetPos(glm::vec3 pos) {
    _position = pos;
}
void FPSCamera::SetFov(float fov) {
    _fov = fov;
    if (_fov > 179.0f) {
        _fov = 179.0f;
    }
    if (_fov < 1.0f) {
        _fov = 1.0f;
    }
    UpdateProjectionMatrix();
}
void FPSCamera::SetAspect(float aspect) {
    _aspect = aspect;
    UpdateProjectionMatrix();
}
void FPSCamera::SetNearPlane(float dist) {
    _near_plane = dist;
}
void FPSCamera::SetFarPlane(float dist) {
    _far_plane = dist;
}

void FPSCamera::UpdateProjectionMatrix() {
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(_fov), _aspect, 0.1f, 10000000.0f);
    _projection_matrix = projection;
}
void FPSCamera::UpdateViewMatrix() {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 buff = glm::mat4(1.0f);
    buff[0][0] = _right.x;
    buff[1][0] = _right.y;
    buff[2][0] = _right.z;
    buff[0][1] = _up.x;
    buff[1][1] = _up.y;
    buff[2][1] = _up.z;
    buff[0][2] = -_direction.x;
    buff[1][2] = -_direction.y;
    buff[2][2] = -_direction.z;

    view = view * buff;
    view = glm::translate(view, -_position);
    _view_matrix = view;
}