#pragma once

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class SkyBox {
    public:
        SkyBox() = default;
        SkyBox(Ref<TextureData> texture);
        SkyBox(Ref<CubeMap> cubemap);

        Ref<CubeMap> GetMap();
        void         ApplyIBL(Ref<Shader> shader);

    private:
        Ref<CubeMap> GenRawCubeMap(Ref<TextureData> texture);
        void         BakeIBL();

        Ref<CubeMap> m_Raw;
        Ref<CubeMap> m_Irradiance;
        Ref<CubeMap> m_Prefilter;
    };
}  // namespace DE