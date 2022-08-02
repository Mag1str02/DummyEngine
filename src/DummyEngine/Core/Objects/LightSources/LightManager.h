#pragma once

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/System.hpp"
#include "Core/Scene/Components.h"
#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Objects/LightSources/DirectionalLight.h"
#include "Core/Objects/LightSources/PointLight.h"
#include "Core/Objects/LightSources/SpotLight.h"

namespace DE
{

    class LightManager : public System
    {
    private:
        void ActivatePointLight(Ref<Shader> shader, const PointLight& point_light, int id);
        void ActivateDirectionalLight(Ref<Shader> shader, const DirectionalLight& directional, int id);
        void ActivateSpotLight(Ref<Shader> shader, const SpotLight& spot_light, int id);

    public:
        LightManager();
        virtual std::string GetName() const override;

        void Update(double dt) override;
    };
}  // namespace DE