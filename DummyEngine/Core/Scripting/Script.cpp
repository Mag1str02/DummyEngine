#include "DummyEngine/Core/Scripting/Script.h"

namespace DE {

    std::string ScriptFieldTypeToString(ScriptFieldType type) {
        switch (type) {
            case ScriptFieldType::None: return "None";
            case ScriptFieldType::Float: return "Float";
            case ScriptFieldType::Double: return "Double";
            case ScriptFieldType::Bool: return "Boolean";
            case ScriptFieldType::String: return "String";
            case ScriptFieldType::S32: return "I32";
            case ScriptFieldType::S64: return "I64";
            case ScriptFieldType::U32: return "UI32";
            case ScriptFieldType::U64: return "UI64";
            case ScriptFieldType::Vec2: return "Vec2";
            case ScriptFieldType::Vec3: return "Vec3";
            case ScriptFieldType::Vec4: return "Vec4";
            case ScriptFieldType::Entity: return "Entity";
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
    template <> ScriptFieldType TypeToScriptFieldType<S8>() {
        return ScriptFieldType::S8;
    }
    template <> ScriptFieldType TypeToScriptFieldType<S16>() {
        return ScriptFieldType::S16;
    }
    template <> ScriptFieldType TypeToScriptFieldType<S32>() {
        return ScriptFieldType::S32;
    }
    template <> ScriptFieldType TypeToScriptFieldType<S64>() {
        return ScriptFieldType::S64;
    }
    template <> ScriptFieldType TypeToScriptFieldType<U8>() {
        return ScriptFieldType::U8;
    }
    template <> ScriptFieldType TypeToScriptFieldType<U16>() {
        return ScriptFieldType::U16;
    }
    template <> ScriptFieldType TypeToScriptFieldType<U32>() {
        return ScriptFieldType::U32;
    }
    template <> ScriptFieldType TypeToScriptFieldType<U64>() {
        return ScriptFieldType::U64;
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
    template <> ScriptFieldType TypeToScriptFieldType<Entity>() {
        return ScriptFieldType::Entity;
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

    ScriptFieldType Script::GetFieldType(const std::string& name) const {
        if (GetClassFields().contains(name)) {
            return GetClassFields().at(name).type;
        }
        return ScriptFieldType::None;
    }
    bool Script::HasField(const std::string& name) const {
        return GetClassFields().contains(name);
    }
    bool Script::AttachedToScene() const {
        return !m_Scene.expired() && m_Entity.Valid();
    }
    Ref<Scene> Script::GetScene() const {
        return m_Scene.lock();
    }
}  // namespace DE
