#include "DummyEngine/Core/Scripting/Script.h"

namespace DE
{

    template <typename T> ScriptFieldType TypeToScriptFieldType(const T& value) { return ScriptFieldType::None; }
    template <> ScriptFieldType           TypeToScriptFieldType<float>(const float& value) { return ScriptFieldType::Float; }
    template <> ScriptFieldType           TypeToScriptFieldType<double>(const double& value) { return ScriptFieldType::Double; }
    template <> ScriptFieldType           TypeToScriptFieldType<bool>(const bool& value) { return ScriptFieldType::Bool; }
    template <> ScriptFieldType           TypeToScriptFieldType<std::string>(const std::string& value) { return ScriptFieldType::String; }
    template <> ScriptFieldType           TypeToScriptFieldType<int32_t>(const int32_t& value) { return ScriptFieldType::I32; }
    template <> ScriptFieldType           TypeToScriptFieldType<int64_t>(const int64_t& value) { return ScriptFieldType::I64; }
    template <> ScriptFieldType           TypeToScriptFieldType<uint32_t>(const uint32_t& value) { return ScriptFieldType::UI32; }
    template <> ScriptFieldType           TypeToScriptFieldType<uint64_t>(const uint64_t& value) { return ScriptFieldType::UI64; }
    template <> ScriptFieldType           TypeToScriptFieldType<Vec2>(const Vec2& value) { return ScriptFieldType::Vec2; }
    template <> ScriptFieldType           TypeToScriptFieldType<Vec3>(const Vec3& value) { return ScriptFieldType::Vec3; }
    template <> ScriptFieldType           TypeToScriptFieldType<Vec4>(const Vec4& value) { return ScriptFieldType::Vec4; }
}  // namespace DE
