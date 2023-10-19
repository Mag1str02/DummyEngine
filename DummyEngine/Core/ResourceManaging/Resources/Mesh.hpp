#pragma once

#include "DummyEngine/Core/Animations/Animation.h"
#include "DummyEngine/Core/ResourceManaging/Resource.hpp"
#include "Material.hpp"
namespace DE {

    class SubMesh: public Resource {
    public:
        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

    private:
        // RAM
        std::vector<Vertex3D> m_Vertices;
        std::vector<U32> m_Indices;
        Material m_Material;
    };

    class Mesh: public Resource {
    public:
        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

    private:
        // RAM
        std::vector<SubMesh> m_SubMeshes;
        Ref<Animation> m_Animation;
        // Loading params
        Path m_Path;
        bool m_FlipUVs;
        bool m_Compress;
    };

}