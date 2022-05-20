#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Addition/types.h"
#include "../VBO/vbo.h"

namespace DE {

class VertexArray {

public:
    void Bind() const;
    void UnBind() const;

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
public:
    BufferId _buffer_id;
};
}  // namespace DE