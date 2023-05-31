#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"
#include "DummyEngine/Core/ResourceManaging/RawData.h"

namespace DE {
#define GL_STRING(name) reinterpret_cast<const char*>(glGetString(name))

    enum class RenderPrimitive;

    GLenum GLTextureFormatInternal(Texture::Format format, Texture::Channels channels);
    GLenum GLTextureFormatExternal(Texture::Channels format);
    GLenum GLDataType(TextureFormat format);
    GLenum RenderPrimitiveToGL(RenderPrimitive primitive);
}  // namespace DE