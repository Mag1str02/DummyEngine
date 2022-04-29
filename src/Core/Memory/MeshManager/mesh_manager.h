#pragma once

#include <map>

#include "../../Structs/Meshes/mesh.h"

class MeshManager
{
private:
    std::map<std::string, std::vector<Mesh>> _models;
    std::map<std::string, std::string> _file_to_name;

    MeshManager();
    static MeshManager &Get();

    void IAddModel(std::string name, fs::path path);
    void ICompressModel(std::string name);
    void IClear();
    void IDumbModels();

public:
    static void AddModel(std::string name, fs::path path);
    static void CompressModel(std::string name);
    static void Clear();
    static void DumbModels();

};