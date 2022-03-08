#include "mesh.h"

Mesh::Mesh(){

};
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) {
    Init(vertices, indices, textures);
}
void Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    indices_amount_ = indices.size();

    BuildMesh();
}

void Mesh::BuildMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    indices.clear();
    vertices.clear();
}

void Mesh::Draw(ShaderProgram& shader_program) {
    shader_program.Use();
    unsigned int diffuse_cnt = 1;
    unsigned int specular_cnt = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].GetType();
        if (name == "texture_diffuse") {
            number = std::to_string(diffuse_cnt++);
        } else if (name == "texture_specular") {
            number = std::to_string(specular_cnt++);
        }
        shader_program.SetInt(("material." + name + number).c_str(), i + 1);
        textures[i].BindToUnit(i + 1);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    shader_program.Use();
    glDrawElements(GL_TRIANGLES, indices_amount_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}