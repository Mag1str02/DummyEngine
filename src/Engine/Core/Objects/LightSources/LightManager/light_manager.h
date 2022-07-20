#pragma once

#include <vector>

#include "../../../ECS/Entity/entity.hpp"
#include "../../../ECS/System/system.hpp"
#include "../../../Rendering/RendererOpenGL/ShaderProgram/shader_program.h"
#include "../DirectionalLight/directional_light.h"
#include "../PointLight/point_light.h"
#include "../SpotLight/spot_light.h"


namespace DE {

struct UniqueShader {
    ShaderProgram shader_program;
};

class LightManager : public System {
private:
    void ActivatePointLight(ShaderProgram& shader_program, const PointLight& point_light, int id);
    void ActivateDirectionalLight(ShaderProgram& shader_program, const DirectionalLight& directional, int id);
    void ActivateSpotLight(ShaderProgram& shader_program, const SpotLight& spot_light, int id);

public:
    LightManager();

    void Update(double dt) override;
};
}  // namespace DE