#include "model_manager.h"

#include "../../UnSorted/Logger/logger.h"

ModelManager::ModelManager() {
}
ModelManager& ModelManager::Get() {
    static ModelManager model_manager;
    return model_manager;
}

void ModelManager::IAddModel(const std::string& name, const std::vector<Mesh>& meshes) {
    if (_models.find(name) != _models.end()) {
        return;
    }
    for (size_t i = 0; i < meshes.size(); ++i) {
        _models[name].AddMesh(meshes[i]);
    }
}
const Model* ModelManager::IGetModel(const std::string& name) {
    if (_models.find(name) != _models.end()) {
        Logger::Info("loading", "ModelManager", "Model (" + name + ") found.");
        return &_models[name];
    } else {
        Logger::Info("loading", "ModelManager", "Model (" + name + ") not found.");
        return nullptr;
    }
}

void ModelManager::AddModel(const std::string& name, const std::vector<Mesh>& meshes) {
    Get().IAddModel(name, meshes);
}
const Model* ModelManager::GetModel(const std::string& name) {
    return Get().IGetModel(name);
}
