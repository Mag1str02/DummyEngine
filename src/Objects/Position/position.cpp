#include "position.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

void Position::UpdateModelMatrix() {
    _model_matrix = glm::mat4(1.0f);
    _model_matrix = glm::translate(_model_matrix, _pos + _pos_offset);
    _model_matrix *= _rotation_matrix;
    _model_matrix = glm::scale(_model_matrix, _scale);
    _update_model = false;
}
void Position::UpdateRotationMatrix() {
    _rotation_matrix = glm::mat4(1.0f);
    _rotation_matrix = glm::rotate(_rotation_matrix, _angles.z + _angles_offset.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _rotation_matrix = glm::rotate(_rotation_matrix, _angles.y + _angles_offset.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _rotation_matrix = glm::rotate(_rotation_matrix, _angles.x + _angles_offset.x, glm::vec3(1.0f, 0.0f, 0.0f));
    UpdateModelMatrix();
    _update_rotation = false;
}

Position::Position()
    : _scale(glm::vec3(1.0f)),                     //
      _update_model(true),                         //
      _update_rotation(true),                      //
      _pos(glm::vec3(0.0f, 0.0f, 0.0f)),           //
      _pos_offset(glm::vec3(0.0f, 0.0f, 0.0f)),    //
      _angles(glm::vec3(0.0f, 0.0f, 0.0f)),        //
      _angles_offset(glm::vec3(0.0f, 0.0f, 0.0f))  //
{
}

void Position::SetScale(glm::vec3 scale) {
    _scale = scale;
    _update_model = true;
}
void Position::SetScale(float scale) {
    _scale = glm::vec3(scale);
    _update_model = true;
}
void Position::SetPosOffset(glm::vec3 offset) {
    _pos_offset = offset;
    _update_model = true;
}
void Position::SetPos(glm::vec3 pos) {
    _pos = pos;
    _update_model = true;
}
void Position::SetPitch(float pitch) {
    _angles.x = pitch;
    _update_rotation = true;
}
void Position::SetYaw(float yaw) {
    _angles.y = yaw;
    _update_rotation = true;
}
void Position::SetRoll(float roll) {
    _angles.z = roll;
    _update_rotation = true;
}
void Position::SetAngles(glm::vec3 angles) {
    _angles = angles;
    _update_rotation = true;
}
void Position::SetAnglesOffset(glm::vec3 angles) {
    _angles_offset = angles;
    _update_rotation = true;
}

glm::mat4 Position::GetModelMatrix() const {
    return _model_matrix;
}
glm::mat4 Position::GetRotationMatrix() const {
    return _rotation_matrix;
}
glm::vec3 Position::GetPos() const {
    return _pos;
}
glm::vec3 Position::GetPosOffset() const {
    return _pos_offset;
}
glm::vec3 Position::GetAngles() const {
    return _angles;
}
float Position::GetPitch() const {
    return _angles.x;
}
float Position::GetYaw() const {
    return _angles.y;
}
float Position::GetRoll() const {
    return _angles.z;
}
glm::vec3 Position::GetScale() const {
    return _scale;
}

void Position::Update() {
    if (_update_rotation) {
        UpdateRotationMatrix();
    }
    if (_update_model) {
        UpdateModelMatrix();
    }
}
