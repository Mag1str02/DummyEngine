#include "gl_vao.h"


namespace DE {

//*----------------------------------------------------------------------------------------------------

void GLVertexArray::Bind() const {
    glBindVertexArray(_buffer_id);
}
void GLVertexArray::UnBind() const {
    glBindVertexArray(0);
}

//*--------------------------------------------------

unsigned int GLVertexArray::VertexArrayManager::CreateVertexArray() {
    ::GLuint buffer_id;
    glGenVertexArrays(1, &buffer_id);
    _reference_count[buffer_id] = 1;
    return buffer_id;
}
unsigned int GLVertexArray::VertexArrayManager::CreateInstance(unsigned int buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void GLVertexArray::VertexArrayManager::DestroyInstance(unsigned int buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyVertexArray(buffer_id);
    }
}
void GLVertexArray::VertexArrayManager::DestroyVertexArray(unsigned int buffer_id) {
    glDeleteVertexArrays(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

GLVertexArray::GLVertexArray() {
    _buffer_id = VertexArrayManager::Get().CreateVertexArray();
}
GLVertexArray::GLVertexArray(const GLVertexArray& other) {
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
}
GLVertexArray::GLVertexArray(GLVertexArray&& other) {
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
}
GLVertexArray& GLVertexArray::operator=(const GLVertexArray& other) {
    if (&other == this) {
        return *this;
    }
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLVertexArray& GLVertexArray::operator=(GLVertexArray&& other) {
    if (&other == this) {
        return *this;
    }
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLVertexArray::~GLVertexArray() {
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

GLVertexArray::VertexArrayManager::VertexArrayManager() {
    _initialized = false;
}
GLVertexArray::VertexArrayManager& GLVertexArray::VertexArrayManager::Get() {
    static VertexArrayManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void GLVertexArray::VertexArrayManager::Initialize() {
    _initialized = true;
}
void GLVertexArray::VertexArrayManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE