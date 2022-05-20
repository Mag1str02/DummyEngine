#include "ebo.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void ElementBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
}
void ElementBuffer::AllocateStorage(GLsizeiptr size, const GLvoid* data, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
void ElementBuffer::AllocateStorage(const std::vector<unsigned int>& storage, GLenum usage) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, storage.size() * sizeof(unsigned int), &storage[0], usage);
}
void ElementBuffer::SetSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

//*--------------------------------------------------

BufferId ElementBuffer::ElementBufferManager::CreateElementBuffer() {
    ::GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    return buffer_id;
}
BufferId ElementBuffer::ElementBufferManager::CreateInstance(BufferId buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void ElementBuffer::ElementBufferManager::DestroyInstance(BufferId buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyElementBuffer(buffer_id);
    }
}
void ElementBuffer::ElementBufferManager::DestroyElementBuffer(BufferId buffer_id) {
    glDeleteBuffers(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

ElementBuffer::ElementBuffer() {
    _buffer_id = ElementBufferManager::Get().CreateElementBuffer();
}
ElementBuffer::ElementBuffer(const ElementBuffer& other) {
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
}
ElementBuffer::ElementBuffer(ElementBuffer&& other) {
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
}
ElementBuffer& ElementBuffer::operator=(const ElementBuffer& other) {
    if (&other == this) {
        return *this;
    }
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
ElementBuffer& ElementBuffer::operator=(ElementBuffer&& other) {
    if (&other == this) {
        return *this;
    }
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = ElementBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
ElementBuffer::~ElementBuffer() {
    ElementBufferManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

ElementBuffer::ElementBufferManager::ElementBufferManager() {
    _initialized = false;
}
ElementBuffer::ElementBufferManager& ElementBuffer::ElementBufferManager::Get() {
    static ElementBufferManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void ElementBuffer::ElementBufferManager::Initialize() {
    _initialized = true;
}
void ElementBuffer::ElementBufferManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE