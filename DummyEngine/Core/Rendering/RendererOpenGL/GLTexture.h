#pragma once

#include <glad/glad.h>

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"

namespace DE
{

    class GLTexture : public Texture
    {
    public:
        GLTexture() = delete;
        GLTexture(const GLTexture& other) = delete;
        GLTexture(GLTexture&& other) = delete;
        GLTexture& operator=(const GLTexture& other) = delete;
        GLTexture& operator=(GLTexture&& other) = delete;

        GLTexture(U32 width, U32 height, TextureChannels format);
        GLTexture(const TextureData& data);

        virtual ~GLTexture();

        virtual U32 Width() const override;
        virtual U32 Height() const override;
        virtual U32 RendererId() const override;

        virtual void SetData(const void* data, U32 size) override;

        virtual void Bind(U32 slot) const override;

    private:
        friend class GLFrameBuffer;
        
        U32 m_Width;
        U32 m_Height;
        GLuint m_TextureId;
        GLenum m_InternalFormat;
        GLenum m_Format;
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