#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../../Addition/types.h"
#include "../../../RenderData/render_data.h"

namespace DE::GLRenderer {

class GLTexture2D {

public:
    void SetData(const Texture2DData& data);
    void BindToUnit(unsigned int unit_id) const;

    GLTexture2D();
    GLTexture2D(const GLTexture2D& other);
    GLTexture2D(GLTexture2D&& other);
    GLTexture2D& operator=(const GLTexture2D& other);
    GLTexture2D& operator=(GLTexture2D&& other);
    ~GLTexture2D();

private:
    friend class Initializer;

    class Texture2DManager {
    private:
        bool _initialized;
        std::unordered_map<unsigned int, int64_t> _reference_count;

        Texture2DManager();

        unsigned int ICreateTexture2D();
        unsigned int ICreateInstance(unsigned int texture_id);
        void IDestroyInstance(unsigned int texture_id);
        void IDestroyTexture2D(unsigned int texture_id);

        static Texture2DManager& Get();

    public:
        static void Initialize();
        static void Terminate();

        static unsigned int CreateTexture2D();
        static unsigned int CreateInstance(unsigned int texture_id);
        static void DestroyInstance(unsigned int texture_id);
        static void DestroyTexture2D(unsigned int texture_id);
    };

    GLint Texture2DFormatToGLenum(Texture2DFormat format) const;

    unsigned int _texture_id;
};
}  // namespace DE