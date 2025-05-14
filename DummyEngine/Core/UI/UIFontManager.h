#pragma once

#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace DummyEngine {

struct GlyphInfo {
    glm::vec2 Size;        // Размеры глифа
    glm::vec2 Bearing;     // Смещение от базовой линии
    float Advance;         // Расстояние до следующего глифа
    glm::vec4 TexCoords;   // Координаты текстуры (x, y, width, height)
};

class UIFontManager {
public:
    static UIFontManager& GetInstance();

    // Загрузка шрифта из файла
    bool LoadFont(const std::string& fontPath, float fontSize, const std::string& fontName);
    
    // Получение шрифта по имени
    Ref<Texture> GetFontTexture(const std::string& fontName);
    
    // Получение размера шрифта
    float GetFontSize(const std::string& fontName) const;

    // Получение информации о глифе
    const GlyphInfo* GetGlyphInfo(const std::string& fontName, char32_t codepoint) const;

    // Расчет размеров текста
    glm::vec2 CalculateTextSize(const std::string& text, const std::string& fontName) const;

private:
    UIFontManager() = default;
    ~UIFontManager() = default;

    struct FontData {
        Ref<Texture> texture;
        float size;
        std::unordered_map<char32_t, GlyphInfo> glyphs;
        float lineHeight;
        float ascender;
        float descender;
    };

    std::unordered_map<std::string, FontData> m_fonts;
};

} // namespace DummyEngine 