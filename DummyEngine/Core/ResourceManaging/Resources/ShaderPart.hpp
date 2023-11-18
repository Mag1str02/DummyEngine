#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/ResourceManaging/Resource.hpp"

namespace DE {

    class ShaderPart;

    class ShaderPartResource : public Resource, public std::enable_shared_from_this<ShaderPartResource> {
    public:
        friend class ShaderResource;
        ShaderPartResource() = delete;
        ShaderPartResource(Path path, ShaderPartType type);

        ~ShaderPartResource();

        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

        const std::string& Source() const { return m_Source; }
        const ShaderPartType& Type() const { return m_Type; }

    private:
        // GPU
        Ref<ShaderPart> m_ShaderPart;
        // RAM
        std::string m_Source;
        // Loading params
        Path m_Path;
        ShaderPartType m_Type;
    };

}