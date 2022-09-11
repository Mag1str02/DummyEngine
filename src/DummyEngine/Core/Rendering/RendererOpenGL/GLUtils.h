#pragma once

#include <GLAD/glad.h>
#include "Core/ResourceManaging/RawData.h"

namespace DE
{
    enum class RenderPrimitive;

    GLenum TextureFormatToGLTextureFormat(TextureFormat format);
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format);
    GLenum RenderPrimitiveToGL(RenderPrimitive primitive);
}  // namespace DE