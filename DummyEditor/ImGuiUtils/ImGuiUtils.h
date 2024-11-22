#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <imgui.h>

namespace DummyEngine {

    class Event;
    class Entity;
    struct Material;
    class Texture;
    class Scene;

    namespace ImGuiUtils {
        class ScopedID {
        public:
            explicit ScopedID(int cnt);
            explicit ScopedID(const char* str);
            explicit ScopedID(const std::string& str);
            explicit ScopedID(const void* ptr);
            ~ScopedID();
        };

        namespace Constants {
            constexpr S32 kBasicFontSize       = 16;
            constexpr S32 kColorPading         = -1;
            constexpr S32 kDefaultRightPadding = -23;
            constexpr S32 kDefaultLeftPadding  = 25;
        }  // namespace Constants

        void ClampRoundValue(Vec3& vec, float min, float max);
        void ClampRoundValue(Vec4& vec, float min, float max);

        constexpr ImVec4 U32ToColor(U32 color) {
            return {float((color & 0xFF0000) >> 16) / 255,
                    float((color & 0xFF00) >> 8) / 255,
                    float(color & 0xFF) / 255,
                    float((color & 0xFF000000) >> 24) / 255};
        }

        enum class PropertyType {
            None = 0,
            Color,
            Pos,
        };

        void EditProperty(
            std::string name, ImGuiDataType type, void* value, float speed = 1.0f, const void* min = nullptr, const void* max = nullptr);
        void EditProperty(const std::string& name, float& value);
        void EditProperty(const std::string& name, double& value);
        void EditProperty(const std::string& name, U8& value);
        void EditProperty(const std::string& name, S8& value);
        void EditProperty(const std::string& name, U16& value);
        void EditProperty(const std::string& name, S16& value);
        void EditProperty(const std::string& name, U32& value);
        void EditProperty(const std::string& name, S32& value);
        void EditProperty(const std::string& name, U64& value);
        void EditProperty(const std::string& name, S64& value);
        void EditProperty(const std::string& name, bool& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, std::string& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec2& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec3& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(std::string name, Vec4& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, ImVec4& vec, PropertyType property_type = PropertyType::None);
        void EditProperty(const std::string& name, Entity& entity, Ref<Scene> scene);
        void EditProperty(Material& mat);

        void EditTexture(const std::string& name, Ref<Texture> texture, const ImVec2 tex_size = {128, 128});
    }  // namespace ImGuiUtils

}  // namespace DummyEngine
