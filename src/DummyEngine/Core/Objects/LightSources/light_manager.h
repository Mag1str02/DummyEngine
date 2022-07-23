#pragma once

#include "DummyEngine/Core/ECS/entity.hpp"
#include "DummyEngine/Core/ECS/system.hpp"
#include "DummyEngine/Core/Rendering/Renderer/shader.h"
#include "DummyEngine/Core/Objects/LightSources/directional_light.h"
#include "DummyEngine/Core/Objects/LightSources/point_light.h"
#include "DummyEngine/Core/Objects/LightSources/spot_light.h"


namespace DE {

struct UniqueShader {
    Ref<Shader> shader;
};

class LightManager : public System {
private:
    void ActivatePointLight(Ref<Shader> shader, const PointLight& point_light, int id);
    void ActivateDirectionalLight(Ref<Shader> shader, const DirectionalLight& directional, int id);
    void ActivateSpotLight(Ref<Shader> shader, const SpotLight& spot_light, int id);

public:
    LightManager();

    void Update(double dt) override;
};
}  // namespace DE