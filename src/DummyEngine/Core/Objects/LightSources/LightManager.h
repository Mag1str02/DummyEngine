#pragma once

#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/ECS/System.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Objects/LightSources/DirectionalLight.h"
#include "DummyEngine/Core/Objects/LightSources/PointLight.h"
#include "DummyEngine/Core/Objects/LightSources/SpotLight.h"


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