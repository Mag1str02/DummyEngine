#pragma once

#include <map>

#include "../../Objects/Models/Mesh/mesh.h"

class MeshManager {
private:
    std::map<std::string, std::vector<Mesh>> _models;
    std::map<std::string, std::string> _file_to_name;

    MeshManager();
    static MeshManager& Get();

    void IAddModel(std::string name, fs::path path);
    
public:
};