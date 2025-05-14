#pragma once

#include <memory>
#include <vector>
#include "UIWidgetComponent.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "UIRenderData.h"

namespace DummyEngine {

class UIWidgetComponent;

class UIRenderer {
public:
    static UIRenderer& GetInstance();

    void Initialize();
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void RenderWidget(Entity widget);
    void RenderText(const std::string& text, const std::string& fontName, const glm::vec4& color, const glm::vec2& position);

private:
    UIRenderer() = default;
    ~UIRenderer() = default;

    void SetupOrthographicProjection();
    void UpdateVertexData();
    void RenderToTexture();
    void RenderToScreen();
    void RenderWidgetData(const UIRenderData& data, const glm::vec4& color, Ref<Texture> texture);

    bool m_isInitialized = false;
    std::vector<Entity> m_widgetsToRender;
    
    // Размеры UI
    uint32_t m_uiWidth = 1920;
    uint32_t m_uiHeight = 1080;
    
    // Ресурсы для рендеринга UI
    Ref<FrameBuffer> m_uiFrameBuffer;
    Ref<Shader> m_uiShader;
    Ref<Texture> m_uiTexture;
    
    // Буферы для рендеринга
    Ref<VertexArray> m_vertexArray;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
    
    // Данные для рендеринга
    UIRenderData m_renderData;
};

} // namespace DummyEngine 