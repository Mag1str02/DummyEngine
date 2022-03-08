#include "texture_manager.h"

#include <iostream>


TextureManager::TextureManager() {
}
TextureManager& TextureManager::GetManager() {
    static TextureManager tx;
    return tx;
}

Texture TextureManager::IAddTexture(const fs::path& path_to_file) {
    if (textures.find(path_to_file.string()) == textures.end()) {
        Texture texture;
        texture.Init(path_to_file);
        textures[path_to_file.string()] = texture;
    }
    return textures[path_to_file.string()];
}

Texture TextureManager::AddTexture(const fs::path& path_to_file) {
    return GetManager().IAddTexture(path_to_file);
}