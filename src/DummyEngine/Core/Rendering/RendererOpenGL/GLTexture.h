#pragma once

#include <GLAD/glad.h>

#include "DummyEngine/Addition/Base.h"
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

        GLTexture(uint32_t width, uint32_t height, TextureFormat format);
        GLTexture(const TextureData& data);

        virtual ~GLTexture();

        virtual uint32_t Width() const override;
        virtual uint32_t Height() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot) const override;

    private:
        uint32_t m_Width;
        uint32_t m_Height;
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
    //         std::unordered_map<unsigned int, int64_t> _reference_count;

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