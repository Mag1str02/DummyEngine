#include "GLTexture.h"

#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"
#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    GLTexture::GLTexture(uint32_t width, uint32_t height, Texture::Channels channels, Texture::Format format, bool depth_buffer) :
        width_(0), height_(0), format_(format), channels_(channels) {
        glGenTextures(1, &texture_id_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        // TODO: Move tex parameters somewhere else...
        if (!depth_buffer) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        }
        Resize(width, height);
    }
    GLTexture::GLTexture(Texture::Channels channels, Texture::Format format) : width_(0), height_(0), format_(format), channels_(channels) {
        glGenTextures(1, &texture_id_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    GLTexture::GLTexture(const TextureData& data) :
        width_(data.Width()), height_(data.Height()), format_(Texture::DataFormat(data.Format())), channels_(Texture::DataChannels(data.Channels())) {
        glGenTextures(1, &texture_id_);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Invalidate(GLDataType(data.Format()), data.Data());
    }

    GLTexture::~GLTexture() {
        glDeleteTextures(1, &texture_id_);
    }

    U32 GLTexture::GetWidth() const {
        return width_;
    }
    U32 GLTexture::GetHeight() const {
        return height_;
    }
    U32 GLTexture::GetRendererId() const {
        return texture_id_;
    }
    Texture::Format GLTexture::GetFormat() const {
        return format_;
    }
    Texture::Channels GLTexture::GetChannels() const {
        return channels_;
    }

    void GLTexture::Copy(Ref<FrameBuffer> buffer, U32 attachment_id) {
        GLTexture* source = reinterpret_cast<GLTexture*>(buffer->GetColorAttachment(attachment_id).get());

        height_   = source->height_;
        width_    = source->width_;
        format_   = source->format_;
        channels_ = source->channels_;
        Invalidate();

        buffer->Bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width_, height_);
    }

    void GLTexture::SetFormat(Format format) {
        if (format_ != format) {
            format_ = format;
            Invalidate();
        }
    }
    void GLTexture::SetChannels(Channels channels) {
        if (channels_ != channels) {
            channels_ = channels;
            Invalidate();
        }
    }
    void GLTexture::Resize(U32 width, U32 height) {
        DE_ASSERT(width * height > 0, "Cannot resize texture to size 0");
        if (width_ != width || height_ != height) {
            width_  = width;
            height_ = height;
            Invalidate();
        }
    }
    void GLTexture::Bind(U32 unit_id) const {
        DE_ASSERT(unit_id > 0, "Cannot bind to texture slot 0, because it's reserved by engine");
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }
    void GLTexture::Invalidate(GLenum data_type, const void* data) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GLTextureFormatInternal(format_, channels_), width_, height_, 0, GLTextureFormatExternal(channels_), data_type, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
}  // namespace DummyEngine
