#include "light_manager.h"

#include <iostream>
namespace DE {

void LightManager::ActivatePointLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.ambient", _point_lights[id].GetComponent<PointLight>().ambient);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.diffuse", _point_lights[id].GetComponent<PointLight>().diffuse);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.specular", _point_lights[id].GetComponent<PointLight>().specular);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].position", _point_lights[id].GetComponent<PointLight>().position);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].clq", _point_lights[id].GetComponent<PointLight>().clq);
}
void LightManager::ActivateDirectionalLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.ambient", _directional_lights[id].GetComponent<DirectionalLight>().ambient);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.diffuse", _directional_lights[id].GetComponent<DirectionalLight>().diffuse);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.specular", _directional_lights[id].GetComponent<DirectionalLight>().specular);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].direction", _directional_lights[id].GetComponent<DirectionalLight>().direction);
}
void LightManager::ActivateSpotLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.ambient", _spot_lights[id].GetComponent<SpotLight>().ambient);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.diffuse", _spot_lights[id].GetComponent<SpotLight>().diffuse);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.specular", _spot_lights[id].GetComponent<SpotLight>().specular);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.position", _spot_lights[id].GetComponent<SpotLight>().position);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.clq", _spot_lights[id].GetComponent<SpotLight>().clq);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].direction", _spot_lights[id].GetComponent<SpotLight>().direction);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].outer_cone_cos", _spot_lights[id].GetComponent<SpotLight>().outer_cone_cos);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].inner_cone_cos", _spot_lights[id].GetComponent<SpotLight>().inner_cone_cos);
}

LightManager::LightManager() {
}

void LightManager::AddPointLight(Entity point_light) {
    _point_lights.push_back(point_light);
}
void LightManager::AddDirectionalLight(Entity directional_light) {
    _directional_lights.push_back(directional_light);
}
void LightManager::AddSpotLight(Entity spot_light) {
    _spot_lights.push_back(spot_light);
}

void LightManager::UpdateLights(ShaderProgram& shader_program) {
    shader_program.SetVec3i("light_amount", _directional_lights.size(), _point_lights.size(), _spot_lights.size());

    for (int i = 0; i < _directional_lights.size(); ++i) {
        ActivateDirectionalLight(shader_program, i);
    }
    for (int i = 0; i < _point_lights.size(); ++i) {
        ActivatePointLight(shader_program, i);
    }
    for (int i = 0; i < _spot_lights.size(); ++i) {
        ActivateSpotLight(shader_program, i);
    }
}
}  // namespace DE