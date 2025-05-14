#include "UIContainer.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"

namespace DummyEngine {

UIContainer::UIContainer() {
    LOG_INFO("Initializing UIContainer");
    TextureAsset textureAsset;
    textureAsset.ID = UUID();
    textureAsset.Name = "DefaultContainerTexture";
    textureAsset.LoadingProps.Path = "Assets/Textures/DefaultContainer.png";
    textureAsset.LoadingProps.FlipUV = false;

    LOG_INFO("Adding texture asset");
    if (AssetManager::AddTextureAsset(textureAsset)) {
        LOG_INFO("Loading texture");
        if (ResourceManager::LoadTexture(textureAsset.ID)) {
            m_textureId = textureAsset.ID;
            LOG_INFO("Getting texture");
            if (auto texture = ResourceManager::GetTexture(m_textureId)) {
                m_texture = texture.value();
                LOG_INFO("Texture loaded successfully");
            } else {
                LOG_ERROR("Failed to get texture");
            }
        } else {
            LOG_ERROR("Failed to load texture");
        }
    } else {
        LOG_ERROR("Failed to add texture asset");
    }
    LOG_INFO("UIContainer initialization completed");
}

void UIContainer::SetBackgroundTexture(const std::string& path) {
    TextureAsset textureAsset;
    textureAsset.ID = UUID();
    textureAsset.Name = "ContainerTexture";
    textureAsset.LoadingProps.Path = path;
    textureAsset.LoadingProps.FlipUV = false;

    if (AssetManager::AddTextureAsset(textureAsset)) {
        if (ResourceManager::LoadTexture(textureAsset.ID)) {
            m_textureId = textureAsset.ID;
            if (auto texture = ResourceManager::GetTexture(m_textureId)) {
                m_texture = texture.value();
            }
        }
    }
}

} // namespace DummyEngine 