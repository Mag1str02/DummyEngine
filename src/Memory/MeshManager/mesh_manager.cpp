#include "mesh_manager.h"

MeshManager::MeshManager() {
}

MeshManager& MeshManager::Get() {
    static MeshManager mesh_manager;
    return mesh_manager;
}

void MeshManager::IAddModel(std::string name, fs::path path) {
    if (_file_to_name.find(path.string()) != _file_to_name.end()) {
        return;
    }
    _file_to_name[path.string()] = name;
    //*Read Model.
}