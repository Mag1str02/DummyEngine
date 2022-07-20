#include "gl_vbo.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void GLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
}

void GLVertexBuffer::AllocateStorage(GLsizeiptr size, const GLvoid* data, GLenum usage) {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
void GLVertexBuffer::SetSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
void GLVertexBuffer::SetFloatAttribute(GLuint index, GLint size, unsigned int offset, unsigned int stride, GLboolean normalized) {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
    glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
}

//*--------------------------------------------------

unsigned int GLVertexBuffer::VertexBufferManager::CreateVertexBuffer() {
    ::GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    _reference_count[buffer_id] = 1;
    return buffer_id;
}
unsigned int GLVertexBuffer::VertexBufferManager::CreateInstance(unsigned int buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void GLVertexBuffer::VertexBufferManager::DestroyInstance(unsigned int buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyVertexBuffer(buffer_id);
    }
}
void GLVertexBuffer::VertexBufferManager::DestroyVertexBuffer(unsigned int buffer_id) {
    glDeleteBuffers(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

GLVertexBuffer::GLVertexBuffer() {
    _buffer_id = VertexBufferManager::Get().CreateVertexBuffer();
}
GLVertexBuffer::GLVertexBuffer(const GLVertexBuffer& other) {
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
}
GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& other) {
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
}
GLVertexBuffer& GLVertexBuffer::operator=(const GLVertexBuffer& other) {
    if (&other == this) {
        return *this;
    }
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& other) {
    if (&other == this) {
        return *this;
    }
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLVertexBuffer::~GLVertexBuffer() {
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

GLVertexBuffer::VertexBufferManager::VertexBufferManager() {
    _initialized = false;
}
GLVertexBuffer::VertexBufferManager& GLVertexBuffer::VertexBufferManager::Get() {
    static VertexBufferManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void GLVertexBuffer::VertexBufferManager::Initialize() {
    _initialized = true;
}
void GLVertexBuffer::VertexBufferManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE