#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Addition/types.h"

namespace DE {

class VertexBuffer {

public:

    void Bind();

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

    BufferId _buffer_id;
};
}  // namespace DE