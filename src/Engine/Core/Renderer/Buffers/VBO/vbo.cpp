#include "vbo.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void VertexBuffer::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
}

//*--------------------------------------------------

BufferId VertexBuffer::VertexBufferManager::CreateVertexBuffer() {
    ::GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    return buffer_id;
}
BufferId VertexBuffer::VertexBufferManager::CreateInstance(BufferId buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void VertexBuffer::VertexBufferManager::DestroyInstance(BufferId buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyVertexBuffer(buffer_id);
    }
}
void VertexBuffer::VertexBufferManager::DestroyVertexBuffer(BufferId buffer_id) {
    glDeleteBuffers(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

VertexBuffer::VertexBuffer() {
    _buffer_id = VertexBufferManager::Get().CreateVertexBuffer();
}
VertexBuffer::VertexBuffer(const VertexBuffer& other) {
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
}
VertexBuffer::VertexBuffer(VertexBuffer&& other) {
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
}
VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {
    if (&other == this) {
        return *this;
    }
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    if (&other == this) {
        return *this;
    }
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexBufferManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
VertexBuffer::~VertexBuffer() {
    VertexBufferManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

VertexBuffer::VertexBufferManager::VertexBufferManager() {
    _initialized = false;
}
VertexBuffer::VertexBufferManager& VertexBuffer::VertexBufferManager::Get() {
    static VertexBufferManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void VertexBuffer::VertexBufferManager::Initialize() {
    _initialized = true;
}
void VertexBuffer::VertexBufferManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE