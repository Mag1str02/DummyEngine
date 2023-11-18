#pragma once

#include "DummyEngine/Core/Animations/Animation.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/ResourceManaging/Resource.hpp"
#include "Material.hpp"
namespace DE {

    class SubMesh: public Resource {
    public:
        SubMesh() = delete;
        SubMesh(std::vector<Vertex3D> &vertices, std::vector<U32> &indices, Ref<Material> mat);

        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

    private:
        // GPU(?)
        Ref<RenderSubMesh> m_RenderSubMesh;
        // RAM
        std::vector<Vertex3D> m_Vertices;
        std::vector<U32> m_Indices;
        Ref<Material> m_Material;
    };

    class Mesh: public Resource {
    public:
        void Prepare() override;
        void Load() override;
        void Unload() override;
        void Remove() override;

    private:
        // GPU
        Ref<RenderMesh> m_RenderMesh;
        // RAM
        std::vector<SubMesh> m_SubMeshes;
        Ref<Animation> m_Animation;
        // Loading params
        Path m_Path;
        bool m_FlipUVs;
        bool m_Compress;
    };

}