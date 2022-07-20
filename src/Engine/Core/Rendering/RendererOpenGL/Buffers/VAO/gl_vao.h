#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Initializer/initializer.h"
#include "../VBO/gl_vbo.h"

namespace DE::GLRenderer {

class GLVertexArray {

public:
    void Bind() const;
    void UnBind() const;

    GLVertexArray();
    GLVertexArray(const GLVertexArray& other);
    GLVertexArray(GLVertexArray&& other);
    GLVertexArray& operator=(const GLVertexArray& other);
    GLVertexArray& operator=(GLVertexArray&& other);
    ~GLVertexArray();

private:
    friend class DE::Initializer;

    class VertexArrayManager {
    private:
        bool _initialized;
        std::unordered_map<unsigned int, int64_t> _reference_count;

        VertexArrayManager();

    public:
        static VertexArrayManager& Get();

        unsigned int CreateVertexArray();
        unsigned int CreateInstance(unsigned int buffer_id);
        void DestroyInstance(unsigned int buffer_id);
        void DestroyVertexArray(unsigned int buffer_id);

        void Initialize();
        void Terminate();
    };
public:
    unsigned int _buffer_id;
};
}  // namespace DE