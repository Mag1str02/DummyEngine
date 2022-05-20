#pragma once
#include <GLAD/glad.h>

#include <unordered_map>
#include <vector>

#include "../../../../Addition/types.h"

namespace DE {

class VertexBuffer {

public:
    void Bind() const;
    void AllocateStorage(GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
    void SetSubData(GLintptr offset, GLsizeiptr size, const void* data);
    void SetFloatAttribute(GLuint index, GLint size, unsigned int offset, unsigned int stride, GLboolean normalized = GL_FALSE);

    VertexBuffer();
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer& operator=(const VertexBuffer& other);
    VertexBuffer& operator=(VertexBuffer&& other);
    ~VertexBuffer();

private:
    friend class Initializer;

    class VertexBufferManager {
    private:
        bool _initialized;
        std::unordered_map<BufferId, ReferenceCount> _reference_count;

        VertexBufferManager();

    public:
        static VertexBufferManager& Get();

        BufferId CreateVertexBuffer();
        BufferId CreateInstance(BufferId buffer_id);
        void DestroyInstance(BufferId buffer_id);
        void DestroyVertexBuffer(BufferId buffer_id);

        void Initialize();
        void Terminate();
    };

public:
    BufferId _buffer_id;
};
}  // namespace DE