#pragma once
#include "Addition/Base.h"
#include "Core/ResourceManaging/RawData.h"

namespace DE
{

    struct TextureLoadingProps
    {
        Path path;
        bool flip;
    };
    struct RenderMeshLoadingProps
    {
        Path path;
        bool flip_uvs;
        bool compress;
    };
    struct TextureAsset
    {
        UUID id;
        std::string name;
        TextureLoadingProps loading_props;
        Ref<TextureData> texture_data;
    };
    struct RenderMeshAsset
    {
        UUID id;
        std::string name;
        RenderMeshLoadingProps loading_props;
        Ref<RenderMeshData> mesh_data;
    };
    struct ShaderAsset
    {
        UUID id;
        std::string name;
        std::vector<ShaderPart> parts;
    };

}  // namespace DE