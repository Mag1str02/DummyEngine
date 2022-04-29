#pragma once
#include <glad/glad.h>

#include <filesystem>
#include <iostream>

#include "../../Structs/Meshes/mesh.h"

namespace fs = std::filesystem;

class VAO
{
private:
    unsigned int _vao, _vbo, _ebo;
    size_t _indices_amount;

public:
    VAO();
    ~VAO();

    VAO(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);
    void Init(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);
    void Init(const Mesh &mesh);

    void SetFloatAttribute(GLuint index, GLint size, GLboolean normalized, unsigned int stride, unsigned int offset);

    void Draw() const;
    void Use() const;

    unsigned int GetVAOId();
    unsigned int GetVBOId();
    unsigned int GetEBOId();

    void Clear();
};