#pragma once

#include <map>
#include <vector>

#include "../../Objects/Model/model.h"
#include "../../Structs/Meshes/mesh.h"
#include "../VAO/vao.h"


namespace DE {
class ModelManager {
private:
    std::map<std::string, Model> _models;

    ModelManager();
    static ModelManager& Get();

    void IAddModel(const std::string& name, const std::vector<Mesh>& meshes);
    const Model* IGetModel(const std::string& name);

public:
    static void AddModel(const std::string& name, const std::vector<Mesh>& meshes);
    static const Model* GetModel(const std::string& name);
};
}  // namespace DE