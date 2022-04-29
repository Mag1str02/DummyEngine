#include "model.h"

#include "../../../ToolBox/Dev/Logger/logger.h"

void Model::BindTextures(ShaderProgram& shader_program, size_t id) const {
    std::string number;
    std::string name;
    std::vector<unsigned int> texture_indices(20, 1);
    for (unsigned int i = 0; i < _textures[id].size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        name = Texture2DTypeToString(_textures[id][i]->GetType());
        number = std::to_string(texture_indices[_textures[id][i]->GetType()]++);
        shader_program.SetInt(("material." + name + number).c_str(), i + 1);
        _textures[id][i]->BindToUnit(i + 1);
    }
    glActiveTexture(GL_TEXTURE0);
}

Model::Model() {
}
void Model::AddMesh(const Mesh& mesh) {
    VAO vao;
    vao.Init(mesh);

    _meshes.push_back(vao);
    _textures.push_back(mesh.textures);
    Logger::Info("loading", "Model", "Created VAO for mesh: " + std::to_string(vao.GetVAOId()));
}
void Model::Draw(ShaderProgram& shader_program) const {
    shader_program.Use();
    for (size_t i = 0; i < _meshes.size(); ++i) {
        BindTextures(shader_program, i);
        _meshes[i].Draw();
    }
    glBindVertexArray(0);
}
