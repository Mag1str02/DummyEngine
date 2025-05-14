#include "UIFontManager.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <glm/glm.hpp>
#include "../../../../Dependencies/UTF8/source/utf8.h"
#include "../../../../Dependencies/STB_IMAGE/stb_truetype.h"

namespace DummyEngine {

UIFontManager& UIFontManager::GetInstance() {
    static UIFontManager instance;
    return instance;
}

bool UIFontManager::LoadFont(const std::string& fontPath, float fontSize, const std::string& fontName) {
    // 1. Загружаем ttf-файл в память
    std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> fontBuffer(size);
    if (!file.read(reinterpret_cast<char*>(fontBuffer.data()), size)) {
        return false;
    }
    file.close();

    // 2. Инициализируем stb_truetype
    stbtt_fontinfo fontInfo;
    if (!stbtt_InitFont(&fontInfo, fontBuffer.data(), 0)) {
        return false;
    }

    // 3. Получаем метрики шрифта
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, fontSize);
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
    float f_ascent = ascent * scale;
    float f_descent = descent * scale;
    float f_lineHeight = (ascent - descent + lineGap) * scale;

    // 4. Собираем глифы и текстуру
    // Список символов (ASCII + кириллица + спецсимволы)
    std::vector<char32_t> chars;
    for (char32_t c = 0x20; c <= 0x7F; ++c) chars.push_back(c); // Basic Latin
    for (char32_t c = 0x0400; c <= 0x04FF; ++c) chars.push_back(c); // Cyrillic
    // Спецсимволы
    chars.push_back(0x2013); chars.push_back(0x2014); // En dash, Em dash
    chars.push_back(0x2018); chars.push_back(0x2019); // Single quotes
    chars.push_back(0x201C); chars.push_back(0x201D); // Double quotes
    chars.push_back(0x2026); // Ellipsis

    // Оценим размеры атласа
    int padding = 2;
    int glyph_w = 0, glyph_h = 0;
    for (char32_t c : chars) {
        int ax, lsb, x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(&fontInfo, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(&fontInfo, c, scale, scale, &x0, &y0, &x1, &y1);
        glyph_w = std::max(glyph_w, x1 - x0);
        glyph_h = std::max(glyph_h, y1 - y0);
    }
    int cols = static_cast<int>(std::ceil(std::sqrt(chars.size())));
    int rows = static_cast<int>(std::ceil(chars.size() / float(cols)));
    int atlas_w = cols * (glyph_w + padding);
    int atlas_h = rows * (glyph_h + padding);
    std::vector<unsigned char> atlas(atlas_w * atlas_h, 0);

    // Рендерим глифы в атлас и сохраняем GlyphInfo
    int x = 0, y = 0, col = 0;
    FontData fontData;
    fontData.size = fontSize;
    fontData.lineHeight = f_lineHeight;
    fontData.ascender = f_ascent;
    fontData.descender = f_descent;
    for (char32_t c : chars) {
        int ax, lsb, x0, y0, x1, y1;
        stbtt_GetCodepointHMetrics(&fontInfo, c, &ax, &lsb);
        stbtt_GetCodepointBitmapBox(&fontInfo, c, scale, scale, &x0, &y0, &x1, &y1);
        int gw = x1 - x0;
        int gh = y1 - y0;
        std::vector<unsigned char> bitmap(gw * gh);
        stbtt_MakeCodepointBitmap(&fontInfo, bitmap.data(), gw, gh, gw, scale, scale, c);
        // Копируем в атлас
        for (int row = 0; row < gh; ++row) {
            std::memcpy(&atlas[(y + row) * atlas_w + x], &bitmap[row * gw], gw);
        }
        // Сохраняем GlyphInfo
        GlyphInfo info;
        info.Size = glm::vec2(gw, gh);
        info.Bearing = glm::vec2(x0, y0);
        info.Advance = ax * scale;
        info.TexCoords = glm::vec4(float(x) / atlas_w, float(y) / atlas_h, float(gw) / atlas_w, float(gh) / atlas_h);
        fontData.glyphs[c] = info;
        // Следующая позиция
        x += glyph_w + padding;
        ++col;
        if (col >= cols) {
            col = 0;
            x = 0;
            y += glyph_h + padding;
        }
    }
    // Создаем TextureData и текстуру
    TextureData texData(atlas.data(), atlas_w, atlas_h, TextureChannels::RED, TextureFormat::U8);
    fontData.texture = Texture::Create(texData);
    m_fonts[fontName] = std::move(fontData);
    return true;
}

Ref<Texture> UIFontManager::GetFontTexture(const std::string& fontName) {
    auto it = m_fonts.find(fontName);
    if (it != m_fonts.end()) {
        return it->second.texture;
    }
    return nullptr;
}

float UIFontManager::GetFontSize(const std::string& fontName) const {
    auto it = m_fonts.find(fontName);
    if (it != m_fonts.end()) {
        return it->second.size;
    }
    return 0.0f;
}

const GlyphInfo* UIFontManager::GetGlyphInfo(const std::string& fontName, char32_t codepoint) const {
    auto it = m_fonts.find(fontName);
    if (it != m_fonts.end()) {
        auto glyphIt = it->second.glyphs.find(codepoint);
        if (glyphIt != it->second.glyphs.end()) {
            return &glyphIt->second;
        }
    }
    return nullptr;
}

glm::vec2 UIFontManager::CalculateTextSize(const std::string& text, const std::string& fontName) const {
    auto it = m_fonts.find(fontName);
    if (it == m_fonts.end()) {
        return glm::vec2(0.0f);
    }

    const auto& fontData = it->second;
    glm::vec2 size(0.0f);
    float currentLineWidth = 0.0f;
    float maxLineWidth = 0.0f;
    float currentLineHeight = fontData.lineHeight;

    // Обрабатываем текст по символам UTF-8
    std::string::const_iterator it_utf8 = text.begin();
    while (it_utf8 != text.end()) {
        char32_t codepoint = utf8::next(it_utf8, text.end());
        
        if (codepoint == '\n') {
            // Новая строка
            maxLineWidth = std::max(maxLineWidth, currentLineWidth);
            currentLineWidth = 0.0f;
            size.y += currentLineHeight;
            currentLineHeight = fontData.lineHeight;
            continue;
        }

        // Получаем информацию о глифе
        const GlyphInfo* glyphInfo = GetGlyphInfo(fontName, codepoint);
        if (glyphInfo) {
            currentLineWidth += glyphInfo->Advance;
            currentLineHeight = std::max(currentLineHeight, 
                glyphInfo->Size.y + glyphInfo->Bearing.y);
        }
    }

    // Учитываем последнюю строку
    maxLineWidth = std::max(maxLineWidth, currentLineWidth);
    size.x = maxLineWidth;
    size.y += currentLineHeight;

    return size;
}

} // namespace DummyEngine 