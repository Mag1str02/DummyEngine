#include "mesh_manager.h"

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../../ToolBox/Loaders/ModelLoader/model_loader.h"
#include "../ModelManager/model_manager.h"
namespace DE {

MeshManager::MeshManager() {
}
MeshManager& MeshManager::Get() {
    static MeshManager mesh_manager;
    return mesh_manager;
}

void MeshManager::IAddModel(std::string name, fs::path path) {
    Logger::Info("loading", "MeshManager", "Loading model (" + name + ").");
    if (_file_to_name.find(path.string()) != _file_to_name.end()) {
        Logger::Info("loading", "MeshManager", "Model (" + name + ") already exists.");
        return;
    }
    _file_to_name[path.string()] = name;
    ModelLoader::ReadModel(path, _models[name], name);
    Logger::Info("loading", "MeshManager", "Model (" + name + ") loaded.");
}
void MeshManager::ICompressModel(std::string name) {
    Mesh mesh;
    mesh = _models[name].front();
    for (size_t i = 0; i < _models[name].size(); ++i) {
        mesh += _models[name][i];
    }
    _models[name].clear();
    _models[name].push_back(mesh);
}
void MeshManager::IClear() {
    _file_to_name.clear();
    _models.clear();
}
void MeshManager::IDumbModels() {
    for (const auto& [name, model] : _models) {
        ModelManager::AddModel(name, model);
    }
    IClear();
}

void MeshManager::AddModel(std::string name, fs::path path) {
    Get().IAddModel(name, path);
}
void MeshManager::CompressModel(std::string name) {
    Get().ICompressModel(name);
}
void MeshManager::Clear() {
    Get().IClear();
}
void MeshManager::DumbModels() {
    Get().IDumbModels();
}
}  // namespace DE