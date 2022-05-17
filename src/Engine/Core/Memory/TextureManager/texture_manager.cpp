#include "texture_manager.h"

#include "../../../ToolBox/Loaders/TextureLoader/texture_loader.h"

namespace DE {
TextureManager::TextureManager() {
}
TextureManager& TextureManager::Get() {
    static TextureManager texture_manager;
    return texture_manager;
}

bool TextureManager::IAddTexture2D(const fs::path& path_to_file, const std::string& name, Texture2DType type) {
    if (_file_to_name.find(path_to_file.string()) != _file_to_name.end()) {
        return false;
    }
    Texture2D texture = TextureLoader::LoadTexture2D(path_to_file, type);
    if (texture.GetType() == Texture2DType::uninitialized) {
        return false;
    }
    _file_to_name[path_to_file.string()] = name;
    _textures_2D[name] = texture;
    return true;
}

const Texture2D* TextureManager::IGetTexure2D(const std::string& name) const {
    if (_textures_2D.find(name) == _textures_2D.end()) {
        return nullptr;
    }
    return &(_textures_2D.at(name));
}

bool TextureManager::AddTexture2D(const fs::path& path_to_file, const std::string& name, Texture2DType type) {
    return Get().IAddTexture2D(path_to_file, name, type);
}

const Texture2D* TextureManager::GetTexure2D(const std::string& name) {
    return Get().IGetTexure2D(name);
}
}  // namespace DE