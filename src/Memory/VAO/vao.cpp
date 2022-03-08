#include "vao.h"

VAO::VAO() {
}
VAO::VAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    Init(vertices, indices);
}
VAO::~VAO() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}
void VAO::Init(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void VAO::SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset) {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(0);
}

void VAO::Use() {
    glBindVertexArray(_vao);
}

unsigned int VAO::GetVAOId() {
    return _vao;
}
unsigned int VAO::GetVBOId() {
    return _vbo;
}
unsigned int VAO::GetEBOId() {
    return _ebo;
}