#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {
    class GLCubeMap : public CubeMap {
        LOG_AUTHOR(GLCubeMap)
    public:
        explicit GLCubeMap(const Ref<TextureData> data);
        GLCubeMap(U32 size, Texture::Format format, Texture::Channels channels, bool gen_mipmap, bool depth_map);
        virtual ~GLCubeMap();

        virtual void   Bind(U32 slot) const override;
        virtual float& GetLOD() override;

        GLCubeMap()                                  = delete;
        GLCubeMap(const GLCubeMap& other)            = delete;
        GLCubeMap(GLCubeMap&& other)                 = delete;
        GLCubeMap& operator=(const GLCubeMap& other) = delete;
        GLCubeMap& operator=(GLCubeMap&& other)      = delete;

    private:
        friend class GLFrameBuffer;
        float  lod_ = 0;
        GLuint map_id_;
    };
}  // namespace DummyEngine