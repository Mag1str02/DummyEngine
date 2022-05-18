#pragma once

#include <glm/glm.hpp>
namespace DE {
class Transformation {
private:
    bool _update_model;
    bool _update_rotation;

    glm::vec3 _scale;
    glm::vec3 _pos, _pos_offset;
    glm::vec3 _angles, _angles_offset;

    glm::mat4 _model_matrix, _rotation_matrix;

    void UpdateModelMatrix();
    void UpdateRotationMatrix();

public:
    Transformation();

    void MoveInWorld(glm::vec3 delta);
    void SetAnglesOffset(glm::vec3 angles);
    void SetAngles(glm::vec3 angles);
    void SetPosOffset(glm::vec3 offset);
    void SetPos(glm::vec3 pos);
    void SetPitch(float pitch);
    void SetYaw(float yaw);
    void SetRoll(float roll);
    void SetScale(float scale);
    void SetScale(glm::vec3 scale);

    glm::mat4 GetModelMatrix() const;
    glm::mat4 GetRotationMatrix() const;
    glm::vec3 GetPos() const;
    glm::vec3 GetPosOffset() const;
    glm::vec3 GetAngles() const;
    float GetPitch() const;
    float GetYaw() const;
    float GetRoll() const;
    glm::vec3 GetScale() const;

    void Update();
};
}  // namespace DE