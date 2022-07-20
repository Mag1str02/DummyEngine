#pragma once
#include <GLAD/glad.h>

#include <unordered_map>
#include <vector>

#include "../../../../Initializer/initializer.h"

namespace DE::GLRenderer  {

class GLElementBuffer{

public:
    void Bind() const;
    void AllocateStorage(GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
    void AllocateStorage(const std::vector<unsigned int>& storage, GLenum usage = GL_STATIC_DRAW);
    void SetSubData(GLintptr offset, GLsizeiptr size, const void* data);

    GLElementBuffer();
    GLElementBuffer(const GLElementBuffer& other);
    GLElementBuffer(GLElementBuffer&& other);
    GLElementBuffer& operator=(const GLElementBuffer& other);
    GLElementBuffer& operator=(GLElementBuffer&& other);
    ~GLElementBuffer();

private:
    friend class DE::Initializer;

    class ElementBufferManager {
    private:
        bool _initialized;
        std::unordered_map<unsigned int, int64_t> _reference_count;

        ElementBufferManager();

    public:
        static ElementBufferManager& Get();

        unsigned int CreateElementBuffer();
        unsigned int CreateInstance(unsigned int buffer_id);
        void DestroyInstance(unsigned int buffer_id);
        void DestroyElementBuffer(unsigned int buffer_id);

        void Initialize();
        void Terminate();
    };

public:
    unsigned int _buffer_id;
};
}  // namespace DE