#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Types/UUID.h"

namespace DummyEngine {

    struct TextureAsset {
        struct LoadingProperties {
            Path          Path;
            bool          FlipUV = true;
            TextureFormat Format = TextureFormat::U8;
        };

        UUID              ID;
        std::string       Name;
        LoadingProperties LoadingProps;
    };
    struct RenderMeshAsset {
        struct LoadingProperties {
            Path Path;
            bool FlipUV;
            bool Compress;
        };
        LoadingProperties LoadingProps;
        UUID              ID;
        std::string       Name;
    };
    struct ShaderAsset {
        UUID                    ID;
        std::string             Name;
        std::vector<ShaderPart> Parts;
    };
    struct ScriptAsset {
        UUID        ID;
        std::string Name;
        Path        Path;
    };

}  // namespace DummyEngine