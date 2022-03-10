#pragma once

#include <vector>
#include <map>
#include "../../Objects/Models/Mesh/mesh.h"
#include "../VAO/vao.h"
#include "../../Objects/Models/Model/model.h"

class ModelManager
{
private:
    std::map<std::string, Model> _models;

    ModelManager();
    static ModelManager &Get();

    void IAddModel(const std::string &name, const std::vector<Mesh> &meshes);
    const Model *IGetModel(const std::string &name);

public:
    static void AddModel(const std::string &name, const std::vector<Mesh> &meshes);
    static const Model* GetModel(const std::string &name);
};