#include "vao.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

//*--------------------------------------------------

BufferId VertexArray::VertexArrayManager::CreateVertexArray() {
    ::GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    return buffer_id;
}
BufferId VertexArray::VertexArrayManager::CreateInstance(BufferId buffer_id) {
    ++_reference_count[buffer_id];
    return buffer_id;
}
void VertexArray::VertexArrayManager::DestroyInstance(BufferId buffer_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[buffer_id];
    if (_reference_count[buffer_id] == 0) {
        DestroyVertexArray(buffer_id);
    }
}
void VertexArray::VertexArrayManager::DestroyVertexArray(BufferId buffer_id) {
    glDeleteBuffers(1, &buffer_id);
}

//*----------------------------------------------------------------------------------------------------

VertexArray::VertexArray() {
    _buffer_id = VertexArrayManager::Get().CreateVertexArray();
}
VertexArray::VertexArray(const VertexArray& other) {
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
}
VertexArray::VertexArray(VertexArray&& other) {
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
}
VertexArray& VertexArray::operator=(const VertexArray& other) {
    if (&other == this) {
        return *this;
    }
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
VertexArray& VertexArray::operator=(VertexArray&& other) {
    if (&other == this) {
        return *this;
    }
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
    _buffer_id = VertexArrayManager::Get().CreateInstance(other._buffer_id);
    return *this;
}
VertexArray::~VertexArray() {
    VertexArrayManager::Get().DestroyInstance(_buffer_id);
}

//*--------------------------------------------------

VertexArray::VertexArrayManager::VertexArrayManager() {
    _initialized = false;
}
VertexArray::VertexArrayManager& VertexArray::VertexArrayManager::Get() {
    static VertexArrayManager vertex_buffer_manager;
    return vertex_buffer_manager;
}

void VertexArray::VertexArrayManager::Initialize() {
    _initialized = true;
}
void VertexArray::VertexArrayManager::Terminate() {
    _initialized = false;
    _reference_count.clear();
}

}  // namespace DE