#include "GLFrameBuffer.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLCubeMap.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

namespace DummyEngine {

    bool IsColorFormat(Texture::Channels channels) {
        switch (channels) {
            case Texture::Channels::R: return true;
            case Texture::Channels::RG: return true;
            case Texture::Channels::RGB: return true;
            case Texture::Channels::RGBA: return true;
            default: return false;
        }
    }
    bool IsDepthFormat(Texture::Channels channels) {
        switch (channels) {
            case Texture::Channels::Depth: return true;
            default: return false;
        }
    }

    void GLFrameBuffer::Rebuild() {
        for (size_t i = 0; i < color_attachments_.size(); ++i) {
            auto& attachment = color_attachments_[i];
            attachment.Texture->Resize(properties_.Width, properties_.Height);
        }
        depth_attachment_.Texture->Resize(properties_.Width, properties_.Height);
    }

    GLFrameBuffer::GLFrameBuffer(const FrameBufferProperties& properties) : buffer_id_(0), properties_(properties) {
        glGenFramebuffers(1, &buffer_id_);
    }
    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &buffer_id_);
    }

    void GLFrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
    }
    void GLFrameBuffer::UnBind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::AddColorAttachment(Texture::Format format, Texture::Channels channels) {
        // TODO: Switch 8 to GPU capabilities.
        DE_ASSERT(IsColorFormat(channels), "Wrong channels for color attachment");
        DE_ASSERT(color_attachments_.size() <= 8, "No available color attachment slot");
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        color_attachments_.push_back({nullptr});
        color_attachments_.back().Texture = Texture::Create(properties_.Width, properties_.Height, channels, format);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + color_attachments_.size() - 1,
                               GL_TEXTURE_2D,
                               std::dynamic_pointer_cast<GLTexture>(color_attachments_.back().Texture)->texture_id_,
                               0);
    }
    void GLFrameBuffer::AddColorAttachment(Ref<CubeMap> map, U32 side, U32 lod) {
        // TODO: Rework
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        color_attachments_.push_back({nullptr});
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + color_attachments_.size() - 1,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                               std::dynamic_pointer_cast<GLCubeMap>(map)->map_id_,
                               lod);
    }
    void GLFrameBuffer::SetDepthAttachment(Texture::Format format) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        depth_attachment_.Texture = Texture::Create(properties_.Width, properties_.Height, Texture::Channels::Depth, format, true);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(depth_attachment_.Texture)->texture_id_, 0);
    }
    void GLFrameBuffer::SetColorAttachment(Ref<Texture> texture, U32 id) {
        if (color_attachments_.size() <= id) {
            color_attachments_.resize(id + 1);
        }
        color_attachments_[id].Texture = texture;
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(texture)->texture_id_, 0);
    }
    void GLFrameBuffer::SetDepthAttachment(Ref<CubeMap> map) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::dynamic_pointer_cast<GLCubeMap>(map)->map_id_, 0);
    }
    bool GLFrameBuffer::Valid() const {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        return status == GL_FRAMEBUFFER_COMPLETE;
    }
    void GLFrameBuffer::Resize(U32 width, U32 height) {
        if ((properties_.Width != width || properties_.Height != height) && 0 < width && 0 < height) {
            properties_.Width  = width;
            properties_.Height = height;
            Rebuild();
        }
    }
    U32 GLFrameBuffer::GetWidth() const {
        return properties_.Width;
    }
    U32 GLFrameBuffer::GetHeight() const {
        return properties_.Height;
    }

    Ref<Texture> GLFrameBuffer::GetColorAttachment(U32 attachment_id = 0) {
        DE_ASSERT(0 <= attachment_id && attachment_id < color_attachments_.size(), "Color attachment with index {} does not exist", attachment_id);
        return color_attachments_[attachment_id].Texture;
    }
    Ref<Texture> GLFrameBuffer::GetDepthAttachment() {
        return depth_attachment_.Texture;
    }

    const FrameBufferProperties& GLFrameBuffer::GetProperties() const {
        return properties_;
    }

}  // namespace DummyEngine