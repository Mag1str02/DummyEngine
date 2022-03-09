#include "model_loader.h"

#include "../../Objects/Models/Mesh/mesh.h"

ModelLoader::ModelLoader() {
}

ModelLoader& ModelLoader::Get() {
    static ModelLoader model_loader;
    return model_loader;
}

void ModelLoader::IReadModel(const fs::path& path, std::vector<Mesh>& meshes) {
    _meshes_amount = 0;
    _nodes_amount = 0;
    _verices_amount = 0;
    const aiScene* scene = _importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        Logger::Error("loading", std::string("Failed To Load Scene: (" + path.string() + ")\n") + _importer.GetErrorString());
        return;
    }
    std::string directory = fs::canonical(path / "..").string();
    meshes.resize(ICountMeshes(scene->mRootNode));

    IProcessNode(scene->mRootNode, scene);

    Logger::Info("loading", "Model loaded: " + path.string());
    Logger::Info("loading", "Model properties: Nodes - " + std::to_string(_nodes_amount) + " | Meshes - " + std::to_string(_meshes_amount) +
                                " | Vertices - " + std::to_string(_verices_amount));
}
size_t ModelLoader::ICountMeshes(aiNode* node) {
}