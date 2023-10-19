#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    struct TextureAsset {
        struct LoadingProperties {

        };
        UUID              id;
        std::string       name;
        LoadingProperties loading_props;
    };
    struct RenderMeshAsset {
        struct LoadingProperties {
            Path path;
            bool flip_uvs;
            bool compress;
        };
        LoadingProperties loading_props;
        UUID              id;
        std::string       name;
    };
    struct ShaderAsset {
        UUID                    id;
        std::string             name;
        std::vector<ShaderPart> parts;
    };
    struct ScriptAsset {
        UUID        id;
        std::string name;
        Path        path;
    };
}  // namespace DE