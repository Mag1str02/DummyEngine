#include "gl_texture_2D.h"

#include <iostream>

#include "../../../../../ToolBox/Dev/Logger/logger.h"

namespace DE {

//*----------------------------------------------------------------------------------------------------

void GLTexture2D::SetData(const Texture2DData& texture_data) {
    GLint format = Texture2DFormatToGLenum(texture_data.format);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture_data.width, texture_data.height, 0, format, GL_UNSIGNED_BYTE, *(texture_data.data));
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}
void GLTexture2D::BindToUnit(unsigned int unit_id) const {
    glActiveTexture(GL_TEXTURE0 + unit_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

//*--------------------------------------------------

unsigned int GLTexture2D::Texture2DManager::ICreateTexture2D() {
    ::GLuint texture_id;
    glGenTextures(1, &texture_id);
    _reference_count[texture_id] = 1;
    return texture_id;
}
unsigned int GLTexture2D::Texture2DManager::ICreateInstance(unsigned int texture_id) {
    ++_reference_count[texture_id];
    return texture_id;
}
void GLTexture2D::Texture2DManager::IDestroyInstance(unsigned int texture_id) {
    if (!_initialized) {
        return;
    }
    --_reference_count[texture_id];
    if (_reference_count[texture_id] == 0) {
        IDestroyTexture2D(texture_id);
    }
}
void GLTexture2D::Texture2DManager::IDestroyTexture2D(unsigned int texture_id) {
    glDeleteTextures(1, &texture_id);
    _reference_count.erase(texture_id);
}

//*----------------------------------------------------------------------------------------------------

GLTexture2D::GLTexture2D() {
    _texture_id = Texture2DManager::CreateTexture2D();
}
GLTexture2D::GLTexture2D(const GLTexture2D& other) {
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
}
GLTexture2D::GLTexture2D(GLTexture2D&& other) {
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
}
GLTexture2D& GLTexture2D::operator=(const GLTexture2D& other) {
    if (&other == this) {
        return *this;
    }
    Texture2DManager::DestroyInstance(_texture_id);
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
    return *this;
}
GLTexture2D& GLTexture2D::operator=(GLTexture2D&& other) {
    if (&other == this) {
        return *this;
    }
    Texture2DManager::DestroyInstance(_texture_id);
    _texture_id = Texture2DManager::CreateInstance(other._texture_id);
    return *this;
}
GLTexture2D::~GLTexture2D() {
    Texture2DManager::DestroyInstance(_texture_id);
}

//*--------------------------------------------------

GLTexture2D::Texture2DManager::Texture2DManager() {
    _initialized = false;
}
GLTexture2D::Texture2DManager& GLTexture2D::Texture2DManager::Get() {
    static Texture2DManager texture_2d_manager;
    return texture_2d_manager;
}

unsigned int GLTexture2D::Texture2DManager::CreateTexture2D() {
    return Texture2DManager::Get().ICreateTexture2D();
}
unsigned int GLTexture2D::Texture2DManager::CreateInstance(unsigned int texture_id) {
    return Texture2DManager::Get().ICreateInstance(texture_id);
}

void GLTexture2D::Texture2DManager::DestroyInstance(unsigned int texture_id) {
    Texture2DManager::Get().IDestroyInstance(texture_id);
}
void GLTexture2D::Texture2DManager::DestroyTexture2D(unsigned int texture_id) {
    Texture2DManager::Get().IDestroyTexture2D(texture_id);
}

GLint GLTexture2D::Texture2DFormatToGLenum(Texture2DFormat format) const {
    switch (format) {
        case Texture2DFormat::RED:
            return GL_RED;
        case Texture2DFormat::RGB:
            return GL_RGB;
        case Texture2DFormat::RGBA:
            return GL_RGBA;
        default:
            return GL_RED;
    }
}
}  // namespace DE
