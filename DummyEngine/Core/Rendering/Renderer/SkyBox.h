#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class TextureData;
    class CubeMap;
    class Shader;

    class SkyBox {
    public:
        SkyBox() = default;
        explicit SkyBox(Ref<TextureData> texture);
        explicit SkyBox(Ref<CubeMap> cubemap);

        Ref<CubeMap> GetMap();
        void         ApplyIBL(Ref<Shader> shader);

    private:
        Ref<CubeMap> GenRawCubeMap(Ref<TextureData> texture);
        void         BakeIBL();

        Ref<CubeMap> raw_;
        Ref<CubeMap> irradiance_;
        Ref<CubeMap> prefilter_;
    };

}  // namespace DummyEngine