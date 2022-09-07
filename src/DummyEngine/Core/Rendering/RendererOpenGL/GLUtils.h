#pragma once

#include <GLAD/glad.h>
#include "Core/ResourceManaging/RawData.h"

namespace DE
{
    GLenum TextureFormatToGLTextureFormat(TextureFormat format);
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format);
}  // namespace DE