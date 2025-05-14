#include "UIRenderer.h"
#include "UIWidgetComponent.h"
#include "UIText.h"
#include "UIFontManager.h"
#include "UIContainer.h"
#include "UIButton.h"
#include "UICheckbox.h"
#include "UITextField.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

UIRenderer& UIRenderer::GetInstance() {
    static UIRenderer instance;
    return instance;
}

void UIRenderer::Initialize() {
    if (m_isInitialized) {
        LOG_INFO("UIRenderer: Already initialized");
        return;
    }

    LOG_INFO("UIRenderer: Initializing renderer {}/{}", m_uiWidth, m_uiHeight);
    m_uiShader = ResourceManager::CreateShader("UI", {
        "Assets/Shaders/UI/UI.vert"
    }, {
        "Assets/Shaders/UI/UI.frag"
    });
    LOG_INFO("UIRenderer: UI shader created");

    // Создаем текстуру для UI
    m_uiTexture = Renderer::GetTexture(Renderer::Textures::White);
    
    LOG_INFO("UIRenderer: UI texture created and initialized");

    // Создаем фреймбуфер
    FrameBufferProperties fbProps;
    fbProps.Width = m_uiWidth;
    fbProps.Height = m_uiHeight;
    m_uiFrameBuffer = ResourceManager::CreateFrameBuffer("UI", fbProps);
    m_uiFrameBuffer->Bind();
    m_uiFrameBuffer->AddColorAttachment(Texture::Format::U8, Texture::Channels::RGBA);
    m_uiFrameBuffer->UnBind();
    LOG_INFO("UIRenderer: UI framebuffer created ({}x{})", m_uiWidth, m_uiHeight);

    // Создаем буферы для вершин
    m_vertexArray = ResourceManager::CreateVertexArray("UI");
    
    // Устанавливаем layout вершин
    BufferLayout layout({
        BufferElement(BufferElementType::Float2),  // Position
        BufferElement(BufferElementType::Float2),  // TexCoord
        BufferElement(BufferElementType::Float4)   // Color
    });

    // Создаем начальные данные для полноэкранного квада
    struct UIVertex {
        glm::vec2 Position;
        glm::vec2 TexCoord;
        glm::vec4 Color;
    };

    std::vector<UIVertex> vertices = {
        {{0.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // Bottom-left
        {{1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // Bottom-right
        {{1.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // Top-right
        {{0.0f, 1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}   // Top-left
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    // Создаем вершинный буфер с данными
    m_vertexBuffer = ResourceManager::CreateVertexBuffer("UI", layout, vertices.data(), vertices.size() * sizeof(UIVertex));
    m_vertexArray->AddVertexBuffer(m_vertexBuffer);

    // Создаем индексный буфер
    m_indexBuffer = IndexBuffer::Create(indices.data(), indices.size());
    m_vertexArray->SetIndexBuffer(m_indexBuffer);

    LOG_INFO("UIRenderer: Vertex buffers created");

    LOG_INFO("UIRenderer: Initialization completed");
    m_isInitialized = true;
}

void UIRenderer::Shutdown() {
    if (!m_isInitialized) {
        LOG_INFO("UIRenderer: Not initialized, skipping shutdown");
        return;
    }

    LOG_INFO("UIRenderer: Starting shutdown");
    
    // Сначала отвязываем все буферы
    if (m_vertexArray) {
        m_vertexArray->UnBind();
    }
    
    // Очищаем данные рендеринга
    m_renderData.Vertices.clear();
    m_renderData.Indices.clear();
    m_widgetsToRender.clear();
    
    // Освобождаем ресурсы в правильном порядке
    m_indexBuffer.reset();
    m_vertexBuffer.reset();
    m_vertexArray.reset();
    m_uiFrameBuffer.reset();
    m_uiShader.reset();
    m_uiTexture.reset();
    
    LOG_INFO("UIRenderer: Resources released");

    m_isInitialized = false;
    LOG_INFO("UIRenderer: Shutdown completed");
}

void UIRenderer::BeginFrame() {
    if (!m_isInitialized) {
        LOG_WARNING("UIRenderer: Not initialized, skipping frame begin");
        return;
    }

    LOG_INFO("UIRenderer: Beginning frame");
    m_uiFrameBuffer->Bind();
    Renderer::Clear();
    m_widgetsToRender.clear();
    LOG_INFO("UIRenderer: Frame buffer bound and cleared");
}

void UIRenderer::EndFrame() {
    if (!m_isInitialized) {
        LOG_WARNING("UIRenderer: Not initialized, skipping frame end");
        return;
    }

    LOG_INFO("UIRenderer: Ending frame");
    UpdateVertexData();
    RenderToTexture();
    RenderToScreen();
    LOG_INFO("UIRenderer: Frame completed");
}

void UIRenderer::RenderWidget(Entity widget) {
    if (!m_isInitialized) {
        LOG_WARNING("UIRenderer: Not initialized, skipping widget render");
        return;
    }

    if (!widget.Valid()) {
        LOG_WARNING("UIRenderer: Invalid widget entity");
        return;
    }

    LOG_INFO("UIRenderer: Adding widget to render queue (Entity ID: {})", widget.Get<IDComponent>().Hex());
    m_widgetsToRender.push_back(widget);
}

void UIRenderer::SetupOrthographicProjection() {
    // Настройка ортографической проекции для UI
    Renderer::SetOrthographicProjection(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
}

void UIRenderer::UpdateVertexData() {
    LOG_INFO("UIRenderer: Updating vertex data for {} widgets", m_widgetsToRender.size());
    // Обновляем данные вершин для всех виджетов
    for (const auto& widget : m_widgetsToRender) {
        if (!widget.Valid()) {
            LOG_WARNING("UIRenderer: Invalid widget in vertex data update");
            continue;
        }
        
        // Получаем компонент виджета в зависимости от его типа
        const UIWidgetComponent* widgetComponent = nullptr;
        if (widget.Has<UIContainer>()) {
            widgetComponent = &widget.Get<UIContainer>();
        } else if (widget.Has<UIText>()) {
            widgetComponent = &widget.Get<UIText>();
        } else if (widget.Has<UIButton>()) {
            widgetComponent = &widget.Get<UIButton>();
        } else if (widget.Has<UICheckbox>()) {
            widgetComponent = &widget.Get<UICheckbox>();
        } else if (widget.Has<UITextField>()) {
            widgetComponent = &widget.Get<UITextField>();
        }

        if (!widgetComponent) {
            LOG_ERROR("UIRenderer: Widget has no valid component");
            continue;
        }

        if (widgetComponent->GetType() == UIWidgetType::Text) {
            auto& textWidget = widget.Get<UIText>();
            LOG_INFO("UIRenderer: Updating text widget vertices (Entity ID: {})", widget.Get<IDComponent>().Hex());
            RenderText(textWidget.GetText(), textWidget.GetFont(), textWidget.GetTextColor(), widgetComponent->GetPosition());
        } else {
            // Для остальных типов виджетов создаем прямоугольник
            LOG_INFO("UIRenderer: Updating widget vertices (Entity ID: {})", widget.Get<IDComponent>().Hex());
            auto pos = widgetComponent->GetPosition();
            auto size = widgetComponent->GetSize();
            
            // Добавляем вершины для прямоугольника
            UIVertex v0 = {{pos.x, pos.y, 0.0f}, {0.0f, 0.0f}};
            UIVertex v1 = {{pos.x + size.x, pos.y, 0.0f}, {1.0f, 0.0f}};
            UIVertex v2 = {{pos.x + size.x, pos.y + size.y, 0.0f}, {1.0f, 1.0f}};
            UIVertex v3 = {{pos.x, pos.y + size.y, 0.0f}, {0.0f, 1.0f}};

            size_t baseIndex = m_renderData.Vertices.size();
            m_renderData.Vertices.push_back(v0);
            m_renderData.Vertices.push_back(v1);
            m_renderData.Vertices.push_back(v2);
            m_renderData.Vertices.push_back(v3);

            // Добавляем индексы для двух треугольников
            m_renderData.Indices.push_back(baseIndex);
            m_renderData.Indices.push_back(baseIndex + 1);
            m_renderData.Indices.push_back(baseIndex + 2);
            m_renderData.Indices.push_back(baseIndex);
            m_renderData.Indices.push_back(baseIndex + 2);
            m_renderData.Indices.push_back(baseIndex + 3);
        }
    }
    LOG_INFO("UIRenderer: Vertex data update completed");
}

void UIRenderer::RenderToTexture() {
    LOG_INFO("UIRenderer: Starting texture rendering");
    
    if (!m_isInitialized || !m_uiFrameBuffer || !m_uiShader || !m_vertexArray) {
        LOG_WARNING("UIRenderer: Cannot render to texture - resources not initialized");
        return;
    }
    
    m_uiFrameBuffer->Bind();
    Renderer::Clear();

    m_uiShader->Bind();
    m_vertexArray->Bind();

    // Рендерим каждый виджет
    for (const auto& widget : m_widgetsToRender) {
        if (!widget.Valid()) {
            LOG_WARNING("UIRenderer: Invalid widget in texture rendering");
            continue;
        }
        
        // Получаем компонент виджета в зависимости от его типа
        const UIWidgetComponent* widgetComponent = nullptr;
        if (widget.Has<UIContainer>()) {
            widgetComponent = &widget.Get<UIContainer>();
        } else if (widget.Has<UIText>()) {
            widgetComponent = &widget.Get<UIText>();
        } else if (widget.Has<UIButton>()) {
            widgetComponent = &widget.Get<UIButton>();
        } else if (widget.Has<UICheckbox>()) {
            widgetComponent = &widget.Get<UICheckbox>();
        } else if (widget.Has<UITextField>()) {
            widgetComponent = &widget.Get<UITextField>();
        }

        if (!widgetComponent) {
            LOG_ERROR("UIRenderer: Widget has no valid component");
            continue;
        }

        if (widgetComponent->GetType() == UIWidgetType::Text) {
            auto& textWidget = widget.Get<UIText>();
            LOG_INFO("UIRenderer: Rendering text widget to texture (Entity ID: {})", widget.Get<IDComponent>().Hex());
            RenderText(textWidget.GetText(), textWidget.GetFont(), textWidget.GetTextColor(), widgetComponent->GetPosition());
        } else {
            LOG_INFO("UIRenderer: Rendering widget to texture (Entity ID: {})", widget.Get<IDComponent>().Hex());
            RenderWidgetData(m_renderData, widgetComponent->GetColor(), widgetComponent->GetTexture());
        }
        LOG_INFO("UIRenderer: ending rendering widget");
    }

    // Отвязываем ресурсы в обратном порядке
    m_vertexArray->UnBind();
    m_uiShader->UnBind();
    m_uiFrameBuffer->UnBind();
    
    LOG_INFO("UIRenderer: Texture rendering completed");
}

void UIRenderer::RenderWidgetData(const UIRenderData& /*data*/, const glm::vec4& color, Ref<Texture> texture) {
    LOG_INFO("UIRenderer: Rendering widget data (Color: [{}, {}, {}, {}])", color.r, color.g, color.b, color.a);
    // Устанавливаем цвет и текстуру
    m_uiShader->SetFloat4("u_Color", color);
    if (texture) {
        texture->Bind(1);
        LOG_INFO("UIRenderer: Using custom texture");
    } else {
        LOG_INFO("UIRenderer: binding default texture");
        m_uiTexture->Bind(1);
        LOG_INFO("UIRenderer: Using default texture");
    }

    // Рендерим виджет
    Renderer::Submit(m_vertexArray, m_uiShader);
}

void UIRenderer::RenderText(const std::string& text, const std::string& fontName, const glm::vec4& color, const glm::vec2& position) {
    LOG_INFO("UIRenderer: Rendering text '{}' with font '{}'", text, fontName);
    // Получаем шрифт
    auto fontTexture = UIFontManager::GetInstance().GetFontTexture(fontName);
    if (!fontTexture) {
        LOG_WARNING("UIRenderer: Font texture not found for '{}'", fontName);
        return;
    }

    // TODO: Реализовать рендеринг текста с учетом шрифта
    // Пока просто рендерим прямоугольник с текстурой шрифта
    float fontSize = UIFontManager::GetInstance().GetFontSize(fontName);
    glm::vec2 textSize = glm::vec2(text.length() * fontSize * 0.6f, fontSize);

    LOG_INFO("UIRenderer: Text size calculated: {}x{}", textSize.x, textSize.y);

    // Добавляем вершины для текста
    UIVertex v0 = {{position.x, position.y, 0.0f}, {0.0f, 0.0f}};
    UIVertex v1 = {{position.x + textSize.x, position.y, 0.0f}, {1.0f, 0.0f}};
    UIVertex v2 = {{position.x + textSize.x, position.y + textSize.y, 0.0f}, {1.0f, 1.0f}};
    UIVertex v3 = {{position.x, position.y + textSize.y, 0.0f}, {0.0f, 1.0f}};

    size_t baseIndex = m_renderData.Vertices.size();
    m_renderData.Vertices.push_back(v0);
    m_renderData.Vertices.push_back(v1);
    m_renderData.Vertices.push_back(v2);
    m_renderData.Vertices.push_back(v3);

    // Добавляем индексы для двух треугольников
    m_renderData.Indices.push_back(baseIndex);
    m_renderData.Indices.push_back(baseIndex + 1);
    m_renderData.Indices.push_back(baseIndex + 2);
    m_renderData.Indices.push_back(baseIndex);
    m_renderData.Indices.push_back(baseIndex + 2);
    m_renderData.Indices.push_back(baseIndex + 3);

    // Рендерим текст
    m_uiShader->SetFloat4("u_Color", color);
    fontTexture->Bind(0);
    Renderer::Submit(m_vertexArray, m_uiShader);
    LOG_INFO("UIRenderer: Text rendering completed");
}

void UIRenderer::RenderToScreen() {
    LOG_INFO("UIRenderer: Starting screen rendering");
    
    if (!m_isInitialized || !m_uiShader || !m_uiTexture || !m_vertexArray) {
        LOG_WARNING("UIRenderer: Cannot render to screen - resources not initialized");
        return;
    }
    
    Renderer::Clear();
    
    // Используем шейдер для отрисовки текстуры на экран
    m_uiShader->Bind();
    m_vertexArray->Bind();
    m_uiTexture->Bind(1);  // Используем слот 1 вместо 0
    
    // Рендерим полноэкранный квад
    Renderer::Submit(m_vertexArray, m_uiShader);
    
    // Отвязываем ресурсы в обратном порядке
    m_uiTexture->Unbind();
    m_vertexArray->UnBind();
    m_uiShader->UnBind();
    
    LOG_INFO("UIRenderer: Screen rendering completed");
}

} // namespace DummyEngine 