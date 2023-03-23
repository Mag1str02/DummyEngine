#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    template <typename T, typename U> constexpr size_t OffsetOf(U T::*member) {
        return (char*)&((T*)nullptr->*member) - (char*)nullptr;
    }

    enum class ScriptFieldType {
        None = 0,
        Float,
        Double,
        Bool,
        String,
        I32,
        I64,
        UI32,
        UI64,
        Vec2,
        Vec3,
        Vec4,
    };

    template <typename T> ScriptFieldType TypeToScriptFieldType(const T& value);

    class ScriptField {
    public:
        ScriptField() {}
        ScriptField(const std::string& name, void* ptr, ScriptFieldType type) : m_Name(name), m_Data(ptr), m_Type(type) {}

        void*                    Get() { return m_Data; }
        template <typename T> T& Get() { return *(T*)m_Data; }
        const std::string&       GetName() const { return m_Name; }
        ScriptFieldType          GetType() const { return m_Type; }

    private:
        ScriptFieldType m_Type;
        std::string     m_Name;
        void*           m_Data;
    };

    class Script {
    public:
        Script() : m_Scene(nullptr) {}
        virtual ~Script() {}

        virtual void OnCreate() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnDestroy() {}

        Script(const Script&)            = delete;
        Script(Script&&)                 = delete;
        Script& operator=(const Script&) = delete;
        Script& operator=(Script&&)      = delete;

        std::unordered_map<std::string, ScriptField>& GetFields() { return m_Fields; }
        ScriptField                                   GetField(const std::string& field_name) { return m_Fields[field_name]; }
        void                                          AttachToScene(Scene* scene, Entity entity) {
            m_Scene  = scene;
            m_Entity = entity;
        }

    protected:
        template <typename T> T&   Add(const T& t) { return m_Entity.Add<T>(t); }
        template <typename T> T&   Get() { return m_Entity.Get<T>(); }
        template <typename T> bool Has() { return m_Entity.Has<T>(); }
        Entity                     GetEntityByName(const std::string& name) const { return m_Scene->GetByName(name); };
        void                       AddField(const ScriptField& field) { m_Fields[field.GetName()] = field; }

        friend class Scene;

        std::unordered_map<std::string, ScriptField> m_Fields;
        Scene*                                       m_Scene;
        Entity                                       m_Entity;
    };
}  // namespace DE

#define ADD_FIELD(field) AddField(ScriptField(std::string(#field), &field, TypeToScriptFieldType(field)))

#define SCRIPT_BASE(type)                                       \
    DE_SCRIPT_API Script* type##Create() { return new type(); } \
    DE_SCRIPT_API void    type##Delete(Script* script) { delete script; }
