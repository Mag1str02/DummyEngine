#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"  // IWYU pragma: export
#include "DummyEngine/Core/ResourceManaging/RawData.h"

#include <glad/glad.h>

#define GL_STRING(name) reinterpret_cast<const char*>(glGetString(name))

namespace DummyEngine {

    enum class RenderPrimitive;

    GLenum GLTextureFormatInternal(Texture::Format format, Texture::Channels channels);
    GLenum GLTextureFormatExternal(Texture::Channels format);
    GLenum GLDataType(TextureFormat format);
    GLenum RenderPrimitiveToGL(RenderPrimitive primitive);

}  // namespace DummyEngine