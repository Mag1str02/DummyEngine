#pragma once
#include <GLAD/glad.h>

#include <unordered_map>
#include <vector>

#include "../../../../Initializer/initializer.h"

namespace DE::GLRenderer {

class GLVertexBuffer {

public:
    void Bind() const;
    void AllocateStorage(GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
    void SetSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void SetFloatAttribute(GLuint index, GLint size, unsigned int offset, unsigned int stride, GLboolean normalized = GL_FALSE);

    GLVertexBuffer();
    GLVertexBuffer(const GLVertexBuffer& other);
    GLVertexBuffer(GLVertexBuffer&& other);
    GLVertexBuffer& operator=(const GLVertexBuffer& other);
    GLVertexBuffer& operator=(GLVertexBuffer&& other);
    ~GLVertexBuffer();

private:
    friend class DE::Initializer;

    class VertexBufferManager {
    private:
        bool _initialized;
        std::unordered_map<unsigned int, int64_t> _reference_count;

        VertexBufferManager();

    public:
        static VertexBufferManager& Get();

        unsigned int CreateVertexBuffer();
        unsigned int CreateInstance(unsigned int buffer_id);
        void DestroyInstance(unsigned int buffer_id);
        void DestroyVertexBuffer(unsigned int buffer_id);

        void Initialize();
        void Terminate();
    };

public:
    unsigned int _buffer_id;
};
}  // namespace DE