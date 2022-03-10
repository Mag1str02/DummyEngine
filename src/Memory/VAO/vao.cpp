#include "vao.h"

VAO::VAO()
{
}
VAO::VAO(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
{
    Init(vertices, indices);
}

VAO::~VAO()
{
    // glDeleteVertexArrays(1, &_vao);
    // glDeleteBuffers(1, &_vbo);
    // glDeleteBuffers(1, &_ebo);
}
void VAO::Init(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
{
    _indices_amount = indices.size();
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //Logger::Info("loading", "Created VAO: " + std::to_string(_vao) + " Indicies amount: " + std::to_string(_indices_amount));
}
void VAO::Init(const Mesh &mesh)
{
    _indices_amount = mesh.indices.size();
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &(mesh.vertices[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &(mesh.indices[0]), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //Logger::Info("loading", "Created VAO: " + std::to_string(_vao) + " Indicies amount: " + std::to_string(_indices_amount));
}

void VAO::SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(index, size, GL_FLOAT, normalized, stride * sizeof(float), (void *)(offset * sizeof(float)));
    glEnableVertexAttribArray(index);
}

void VAO::Draw() const
{
    Use();
    glDrawElements(GL_TRIANGLES, _indices_amount, GL_UNSIGNED_INT, 0);
}

void VAO::Use() const
{
    glBindVertexArray(_vao);
}

unsigned int VAO::GetVAOId()
{
    return _vao;
}
unsigned int VAO::GetVBOId()
{
    return _vbo;
}
unsigned int VAO::GetEBOId()
{
    return _ebo;
}
void VAO::Clear()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}