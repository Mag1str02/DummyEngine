#pragma once
#include <GLAD/glad.h>

#include <unordered_map>
#include <vector>

#include "../../../../../Addition/types.h"

namespace DE {

class ElementBuffer {

public:
    void Bind() const;
    void AllocateStorage(GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
    void AllocateStorage(const std::vector<unsigned int>& storage, GLenum usage = GL_STATIC_DRAW);
    void SetSubData(GLintptr offset, GLsizeiptr size, const void* data);

    ElementBuffer();
    ElementBuffer(const ElementBuffer& other);
    ElementBuffer(ElementBuffer&& other);
    ElementBuffer& operator=(const ElementBuffer& other);
    ElementBuffer& operator=(ElementBuffer&& other);
    ~ElementBuffer();

private:
    friend class Initializer;

    class ElementBufferManager {
    private:
        bool _initialized;
        std::unordered_map<BufferId, ReferenceCount> _reference_count;

        ElementBufferManager();

    public:
        static ElementBufferManager& Get();

        BufferId CreateElementBuffer();
        BufferId CreateInstance(BufferId buffer_id);
        void DestroyInstance(BufferId buffer_id);
        void DestroyElementBuffer(BufferId buffer_id);

        void Initialize();
        void Terminate();
    };

public:
    BufferId _buffer_id;
};
}  // namespace DE