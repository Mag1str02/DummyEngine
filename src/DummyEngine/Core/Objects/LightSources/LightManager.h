#pragma once

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/System.hpp"
#include "Core/Scene/Components.h"
#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Objects/LightSources/LightSource.h"

namespace DE
{

    class LightManager : public System
    {
    private:
        void ActivateLightSource(Ref<Shader> shader, const LightSource& source, int id);

    public:
        LightManager();
        virtual std::string GetName() const override;

        void Update(double dt) override;
    };
}  // namespace DE