#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Addition/types.h"
#include "../VBO/vbo.h"


namespace DE {

class VertexArray {

public:
    void SetVertexAttribute(const VertexBuffer& buffer, GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset);
    void SetElementBuffer(const VertexBuffer& buffer, GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset);

    VertexArray();
    VertexArray(const VertexArray& other);
    VertexArray(VertexArray&& other);
    VertexArray& operator=(const VertexArray& other);
    VertexArray& operator=(VertexArray&& other);
    ~VertexArray();

private:
    friend class Initializer;

    class VertexArrayManager {
    private:
        bool _initialized;
        std::unordered_map<BufferId, ReferenceCount> _reference_count;

        VertexArrayManager();

    public:
        static VertexArrayManager& Get();

        BufferId CreateVertexArray();
        BufferId CreateInstance(BufferId buffer_id);
        void DestroyInstance(BufferId buffer_id);
        void DestroyVertexArray(BufferId buffer_id);

        void Initialize();
        void Terminate();
    };

    BufferId _buffer_id;
};
}  // namespace DE