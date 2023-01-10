#pragma once

#include <glad/glad.h>
#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE
{
    enum class RenderPrimitive;

    GLenum TextureFormatToGLTextureFormat(TextureFormat format);
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format);
    GLenum RenderPrimitiveToGL(RenderPrimitive primitive);
}  // namespace DE