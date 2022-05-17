#include "light_manager.h"

#include <iostream>
namespace DE {
void LightManager::ActivatePointLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.ambient", point_lights_[id]->ambient);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.diffuse", point_lights_[id]->diffuse);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].light.specular", point_lights_[id]->specular);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].position", point_lights_[id]->position);
    shader_program.SetVec3f("point_lights[" + std::to_string(id) + "].clq", point_lights_[id]->clq);
}
void LightManager::ActivateDirectionalLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.ambient", directional_lights_[id]->ambient);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.diffuse", directional_lights_[id]->diffuse);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].light.specular", directional_lights_[id]->specular);
    shader_program.SetVec3f("directional_lights[" + std::to_string(id) + "].direction", directional_lights_[id]->direction);
}
void LightManager::ActivateSpotLight(ShaderProgram& shader_program, int id) {
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.ambient", spot_lights_[id]->ambient);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.diffuse", spot_lights_[id]->diffuse);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.light.specular", spot_lights_[id]->specular);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.position", spot_lights_[id]->position);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].point_light.clq", spot_lights_[id]->clq);
    shader_program.SetVec3f("spot_lights[" + std::to_string(id) + "].direction", spot_lights_[id]->direction);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].outer_cone_cos", spot_lights_[id]->outer_cone_cos);
    shader_program.SetFloat("spot_lights[" + std::to_string(id) + "].inner_cone_cos", spot_lights_[id]->inner_cone_cos);
}

LightManager::LightManager() {
}

void LightManager::AddPointLight(const PointLight* point_light) {
    point_lights_.push_back(point_light);
}
void LightManager::AddDirectionalLight(const DirectionalLight* directional_light) {
    directional_lights_.push_back(directional_light);
}
void LightManager::AddSpotLight(const SpotLight* spot_light) {
    spot_lights_.push_back(spot_light);
}

void LightManager::UpdateLights(ShaderProgram& shader_program) {
    shader_program.SetVec3i("light_amount", directional_lights_.size(), point_lights_.size(), spot_lights_.size());

    for (int i = 0; i < directional_lights_.size(); ++i) {
        ActivateDirectionalLight(shader_program, i);
    }
    for (int i = 0; i < point_lights_.size(); ++i) {
        ActivatePointLight(shader_program, i);
    }

    for (int i = 0; i < spot_lights_.size(); ++i) {
        ActivateSpotLight(shader_program, i);
    }
}
}  // namespace DE