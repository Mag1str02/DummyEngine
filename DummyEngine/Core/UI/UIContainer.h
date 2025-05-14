#pragma once

#include "UIWidgetComponent.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Utils/Types/UUID.h"
#include <string>

namespace DummyEngine {

class UIContainer : public UIWidgetComponent {
public:
    UIContainer();
    ~UIContainer() = default;

    void SetBackgroundTexture(const std::string& texturePath);
    Ref<Texture> GetBackgroundTexture() const { return m_texture; }

    UIWidgetType GetType() const override { return UIWidgetType::Container; }

private:
    Ref<Texture> m_texture;
    UUID m_textureId;
};

} // namespace DummyEngine 