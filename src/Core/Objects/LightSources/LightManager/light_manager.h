#pragma once

#include "../DirectionalLight/directional_light.h"
#include "../PointLight/point_light.h"
#include "../SpotLight/spot_light.h"
#include "../../../Wrappings/ShaderProgram/shader_program.h"
#include <vector>

class LightManager {
private:
    std::vector<const PointLight*> point_lights_;
    std::vector<const DirectionalLight*> directional_lights_;
    std::vector<const SpotLight*> spot_lights_;

    void ActivatePointLight(ShaderProgram& shader_program, int id);
    void ActivateDirectionalLight(ShaderProgram& shader_program, int id);
    void ActivateSpotLight(ShaderProgram& shader_program, int id);

public:
    LightManager();

    void AddPointLight(const PointLight* point_light);
    void AddDirectionalLight(const DirectionalLight* directional_light);
    void AddSpotLight(const SpotLight* spot_light);
    
    void UpdateLights(ShaderProgram& shader_program);
};