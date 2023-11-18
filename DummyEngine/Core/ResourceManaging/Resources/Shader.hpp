#pragma once

#include "DummyEngine/Core/ResourceManaging/Resource.hpp"
#include "ShaderPart.hpp"
namespace DE {

    class ShaderResource : public Resource {
    public:
        ~ShaderResource();

        void AddPart(Ref<ShaderPartResource> &part);
        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

    private:
        // GPU
        Ref<Shader> m_Shader;
        // RAM
        std::vector<Ref<ShaderPartResource>> m_Parts;
        std::string m_Name;
    };

}