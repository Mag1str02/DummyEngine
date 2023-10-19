#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/ShaderPart.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class GLShaderPart : public ShaderPart {
    public:
        friend class GLShader;

        GLShaderPart() = delete;
        GLShaderPart(const Shader& other) = delete;
        GLShaderPart(ShaderPart&& other) = delete;
        GLShaderPart& operator=(Shader&& other) = delete;
        GLShaderPart& operator=(const Shader& other) = delete;

        GLShaderPart(const ShaderPartResource&data);
        virtual ~GLShaderPart();
    private:
        GLuint m_ShaderId;
    };

}