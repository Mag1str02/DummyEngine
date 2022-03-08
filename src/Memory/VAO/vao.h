#pragma once
#include <glad/glad.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class VAO {
private:
    unsigned int _vao, _vbo, _ebo;

public:
    VAO();
    ~VAO();

    VAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    void Init(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    void SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset);

    void Use();

    unsigned int GetVAOId();
    unsigned int GetVBOId();
    unsigned int GetEBOId();
};