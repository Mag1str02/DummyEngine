#include "DummyEngine/Core/Scripting/Script.h"

namespace DE {

    std::string ScriptFieldTypeToString(ScriptFieldType type) {
        switch (type) {
            case ScriptFieldType::None: return "None";
            case ScriptFieldType::Float: return "Float";
            case ScriptFieldType::Double: return "Double";
            case ScriptFieldType::Bool: return "Boolean";
            case ScriptFieldType::String: return "String";
            case ScriptFieldType::I32: return "I32";
            case ScriptFieldType::I64: return "I64";
            case ScriptFieldType::UI32: return "UI32";
            case ScriptFieldType::UI64: return "UI64";
            case ScriptFieldType::Vec2: return "Vec2";
            case ScriptFieldType::Vec3: return "Vec3";
            case ScriptFieldType::Vec4: return "Vec4";
            default: return "Unknown";
        }
    }
    template <typename T> ScriptFieldType TypeToScriptFieldType() {
        return ScriptFieldType::None;
    }
    template <> ScriptFieldType TypeToScriptFieldType<float>() {
        return ScriptFieldType::Float;
    }
    template <> ScriptFieldType TypeToScriptFieldType<double>() {
        return ScriptFieldType::Double;
    }
    template <> ScriptFieldType TypeToScriptFieldType<bool>() {
        return ScriptFieldType::Bool;
    }
    template <> ScriptFieldType TypeToScriptFieldType<std::string>() {
        return ScriptFieldType::String;
    }
    template <> ScriptFieldType TypeToScriptFieldType<int32_t>() {
        return ScriptFieldType::I32;
    }
    template <> ScriptFieldType TypeToScriptFieldType<int64_t>() {
        return ScriptFieldType::I64;
    }
    template <> ScriptFieldType TypeToScriptFieldType<uint32_t>() {
        return ScriptFieldType::UI32;
    }
    template <> ScriptFieldType TypeToScriptFieldType<uint64_t>() {
        return ScriptFieldType::UI64;
    }
    template <> ScriptFieldType TypeToScriptFieldType<Vec2>() {
        return ScriptFieldType::Vec2;
    }
    template <> ScriptFieldType TypeToScriptFieldType<Vec3>() {
        return ScriptFieldType::Vec3;
    }
    template <> ScriptFieldType TypeToScriptFieldType<Vec4>() {
        return ScriptFieldType::Vec4;
    }

    bool Script::FieldIterator::operator==(const FieldIterator& other) const {
        return m_Script == other.m_Script && m_Iterator == other.m_Iterator;
    }
    bool Script::FieldIterator::operator!=(const FieldIterator& other) const {
        return m_Script != other.m_Script || m_Iterator != other.m_Iterator;
    }
    Script::FieldIterator& Script::FieldIterator::operator++() {
        ++m_Iterator;
        return *this;
    }
    Script::FieldIterator Script::FieldIterator::operator++(int) {
        FieldIterator result = *this;
        ++m_Iterator;
        return result;
    }
    std::pair<std::reference_wrapper<const std::string>, Script::Field> Script::FieldIterator::operator*() const {
        return {std::cref(m_Iterator->first), Field(m_Iterator->second.type, (char*)m_Script + m_Iterator->second.offset)};
    }

    Script::FieldIterator::FieldIterator(Script* owner, Iterator it) : m_Iterator(it), m_Script(owner) {}

    Script::FieldIterator Script::begin() {
        return FieldIterator(this, GetClassFields().begin());
    }
    Script::FieldIterator Script::end() {
        return FieldIterator(this, GetClassFields().end());
    }
    Script::Field::Field(ScriptFieldType type, void* ptr) : m_Type(type), m_Data(ptr) {}
    void Script::AttachToScene(WeakRef<Scene> scene, Entity entity) {
        m_Scene  = scene;
        m_Entity = entity;
    }
    Entity Script::GetEntityByName(const std::string& name) const {
        auto scene = m_Scene.lock();
        DE_ASSERT(scene != nullptr, "Using invalid scene in script");
        return scene->GetByName(name);
    };

}  // namespace DE
