#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"

namespace DE {
    class GLCubeMap : public CubeMap {
    public:
        GLCubeMap(const Ref<TextureData> data);
        GLCubeMap(U32 size, TextureFormat format, TextureChannels channels);
        virtual ~GLCubeMap();
        virtual void Bind(U32 slot) const override;

        GLCubeMap()                                  = delete;
        GLCubeMap(const GLCubeMap& other)            = delete;
        GLCubeMap(GLCubeMap&& other)                 = delete;
        GLCubeMap& operator=(const GLCubeMap& other) = delete;
        GLCubeMap& operator=(GLCubeMap&& other)      = delete;

    private:
        friend class GLFrameBuffer;
        GLuint m_MapId;
    };
}  // namespace DE