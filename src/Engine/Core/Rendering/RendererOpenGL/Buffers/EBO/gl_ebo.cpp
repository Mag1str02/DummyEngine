#include "gl_ebo.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void GLElementBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
}
void GLElementBuffer::AllocateStorage(GLsizeiptr size, const GLvoid* data, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
void GLElementBuffer::AllocateStorage(const std::vector<unsigned int>& storage, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, storage.size() * sizeof(unsigned int), &(storage[0]), usage);
}
void GLElementBuffer::SetSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

//*--------------------------------------------------

unsigned int GLElementBuffer::ElementBufferManager::CreateElementBuffer() {
    ::GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    _reference_count[buffer_id] = 1;
    return buffer_id;
}
unsigned int GLElementBuffer::ElementBufferManager::CreateInstance(unsigned int buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void GLElementBuffer::ElementBufferManager::DestroyInstance(unsigned int buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyElementBuffer(buffer_id);
    }
}
void GLElementBuffer::ElementBufferManager::DestroyElementBuffer(unsigned int buffer_id) {
    glDeleteBuffers(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

GLElementBuffer::GLElementBuffer() {
    _buffer_id = ElementBufferManager::Get().CreateElementBuffer();
}
GLElementBuffer::GLElementBuffer(const GLElementBuffer& other) {
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
}
GLElementBuffer::GLElementBuffer(GLElementBuffer&& other) {
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
}
GLElementBuffer& GLElementBuffer::operator=(const GLElementBuffer& other) {
    if (&other == this) {
        return *this;
    }
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLElementBuffer& GLElementBuffer::operator=(GLElementBuffer&& other) {
    if (&other == this) {
        return *this;
    }
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
GLElementBuffer::~GLElementBuffer() {
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

GLElementBuffer::ElementBufferManager::ElementBufferManager() {
    _initialized = false;
}
GLElementBuffer::ElementBufferManager& GLElementBuffer::ElementBufferManager::Get() {
    static ElementBufferManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void GLElementBuffer::ElementBufferManager::Initialize() {
    _initialized = true;
}
void GLElementBuffer::ElementBufferManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE