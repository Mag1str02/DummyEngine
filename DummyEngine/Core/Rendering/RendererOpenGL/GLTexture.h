#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/ResourceManaging/Resources/Texture.hpp"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class GLTexture : public Texture {
    public:
        GLTexture()                                  = delete;
        GLTexture(const GLTexture& other)            = delete;
        GLTexture(GLTexture&& other)                 = delete;
        GLTexture& operator=(const GLTexture& other) = delete;
        GLTexture& operator=(GLTexture&& other)      = delete;

        GLTexture(U32 width, U32 height, Channels channels, Format format);
        GLTexture(Texture::Channels channels, Format format);
        GLTexture(const TextureResource& data);

        virtual ~GLTexture();

        virtual U32      GetWidth() const override;
        virtual U32      GetHeight() const override;
        virtual U32      GetRendererId() const override;
        virtual Format   GetFormat() const override;
        virtual Channels GetChannels() const override;

        virtual void Copy(Ref<FrameBuffer> buffer, U32 attachment_id) override;
        virtual void SetFormat(Format format) override;
        virtual void SetChannels(Channels channels) override;
        virtual void Resize(U32 width, U32 height) override;
        virtual void Bind(U32 slot) const override;
        virtual void Remove() override;

    private:
        void Invalidate(GLenum data_type = GL_UNSIGNED_BYTE, const void* data = nullptr);

        friend class GLFrameBuffer;

        GLuint   m_TextureId;
        U32      m_Width;
        U32      m_Height;
        Format   m_Format;
        Channels m_Channels;
    };

    // class GLTexture
    // {
    // public:
    //     void SetData(const Texture2DData& data);
    //     void BindToUnit(unsigned int unit_id) const;

    //     GLTexture();
    //     GLTexture(const GLTexture& other);
    //     GLTexture(GLTexture&& other);
    //     GLTexture& operator=(const GLTexture& other);
    //     GLTexture& operator=(GLTexture&& other);
    //     ~GLTexture();

    // private:
    //     friend class Initializer;

    //     class Texture2DManager
    //     {
    //     private:
    //         bool _initialized;
    //         std::unordered_map<unsigned int, S64> _reference_count;

    //         Texture2DManager();

    //         unsigned int ICreateTexture2D();
    //         unsigned int ICreateInstance(unsigned int texture_id);
    //         void IDestroyInstance(unsigned int texture_id);
    //         void IDestroyTexture2D(unsigned int texture_id);

    //         static Texture2DManager& Get();

    //     public:
    //         static void Initialize();
    //         static void Terminate();

    //         static unsigned int CreateTexture2D();
    //         static unsigned int CreateInstance(unsigned int texture_id);
    //         static void DestroyInstance(unsigned int texture_id);
    //         static void DestroyTexture2D(unsigned int texture_id);
    //     };

    //     GLint Texture2DFormatToGLenum(Texture2DFormat format) const;

    //     unsigned int m_TextureId;
    // };
}  // namespace DE