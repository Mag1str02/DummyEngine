#pragma once

#include <vector>

#include "../../../ECS/Entity/entity.hpp"
#include "../../../Wrappings/ShaderProgram/shader_program.h"
#include "../DirectionalLight/directional_light.h"
#include "../PointLight/point_light.h"
#include "../SpotLight/spot_light.h"


namespace DE {
class LightManager {
private:
    std::vector<Entity> _point_lights;
    std::vector<Entity> _directional_lights;
    std::vector<Entity> _spot_lights;

    void ActivatePointLight(ShaderProgram& shader_program, int id);
    void ActivateDirectionalLight(ShaderProgram& shader_program, int id);
    void ActivateSpotLight(ShaderProgram& shader_program, int id);

public:
    LightManager();

    void AddPointLight(Entity point_light);
    void AddDirectionalLight(Entity directional_light);
    void AddSpotLight(Entity spot_light);

    void UpdateLights(ShaderProgram& shader_program);
};
}  // namespace DE