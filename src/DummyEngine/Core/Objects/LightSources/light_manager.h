#pragma once

#include "DummyEngine/Core/ECS/entity.hpp"
#include "DummyEngine/Core/ECS/system.hpp"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_shader_program.h"
#include "DummyEngine/Core/Objects/LightSources/directional_light.h"
#include "DummyEngine/Core/Objects/LightSources/point_light.h"
#include "DummyEngine/Core/Objects/LightSources/spot_light.h"


namespace DE {

struct UniqueShader {
    GLShaderProgram shader_program;
};

class LightManager : public System {
private:
    void ActivatePointLight(GLShaderProgram& shader_program, const PointLight& point_light, int id);
    void ActivateDirectionalLight(GLShaderProgram& shader_program, const DirectionalLight& directional, int id);
    void ActivateSpotLight(GLShaderProgram& shader_program, const SpotLight& spot_light, int id);

public:
    LightManager();

    void Update(double dt) override;
};
}  // namespace DE