#pragma once

#include <GLAD/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"

namespace DE
{
    class GLCubeMap : public CubeMap
    {
    public:
        GLCubeMap(const Ref<TextureData> data);
        virtual ~GLCubeMap();

        virtual void Bind() const override;

        GLCubeMap() = delete;
        GLCubeMap(const GLCubeMap& other) = delete;
        GLCubeMap(GLCubeMap&& other) = delete;
        GLCubeMap& operator=(const GLCubeMap& other) = delete;
        GLCubeMap& operator=(GLCubeMap&& other) = delete;

    private:
        GLuint m_MapId;
    };
}  // namespace DE