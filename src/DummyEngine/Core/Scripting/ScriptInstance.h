#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ECS/Entity.hpp"

namespace DE
{
    enum class ScriptFieldType
    {
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

    class ScriptField
    {
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

#define ADD_FIELD(field) AddField(ScriptField(std::string(#field), &field, TypeToScriptFieldType(field)))

    class ScriptInstance
    {
    public:
        ScriptInstance() : m_Scene(nullptr) {}
        virtual ~ScriptInstance() {}

        virtual void OnCreate() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnDestroy() {}

        ScriptInstance(const ScriptInstance&)            = delete;
        ScriptInstance(ScriptInstance&&)                 = delete;
        ScriptInstance& operator=(const ScriptInstance&) = delete;
        ScriptInstance& operator=(ScriptInstance&&)      = delete;

        std::unordered_map<std::string, ScriptField>& GetFields() { return m_Fields; }
        ScriptField                                   GetField(const std::string& field_name) { return m_Fields[field_name]; }

    protected:
        template <typename T> T& GetComponent() { return m_Entity.GetComponent<T>(); }
        Entity                   GetEntityByName(const std::string& name) const { return m_Scene->GetByName(name); };
        void                     AddField(const ScriptField& field) { m_Fields[field.GetName()] = field; }

        friend class Scene;

        void AttachToEntity(Scene* scene, Entity entity)
        {
            m_Scene  = scene;
            m_Entity = entity;
        }
        std::unordered_map<std::string, ScriptField> m_Fields;
        Scene*                                       m_Scene;
        Entity                                       m_Entity;
    };
}  // namespace DE

#define SCRIPT_BASE(type)                                  \
    DE_SCRIPT_API void type##Construct(void* adr)          \
    {                                                      \
        new (adr) type();                                  \
    }                                                      \
    DE_SCRIPT_API void type##Destruct(void* adr)           \
    {                                                      \
        reinterpret_cast<type*>(adr)->~type();             \
    }                                                      \
    DE_SCRIPT_API void type##OnCreate(void* adr)           \
    {                                                      \
        reinterpret_cast<type*>(adr)->OnCreate();          \
    }                                                      \
    DE_SCRIPT_API void type##OnUpdate(void* adr, float dt) \
    {                                                      \
        reinterpret_cast<type*>(adr)->OnUpdate(dt);        \
    }                                                      \
    DE_SCRIPT_API void type##OnDestroy(void* adr)          \
    {                                                      \
        reinterpret_cast<type*>(adr)->OnDestroy();         \
    }                                                      \
    DE_SCRIPT_API uint32_t type##AlignOf()                 \
    {                                                      \
        return alignof(type);                              \
    }                                                      \
    DE_SCRIPT_API uint32_t type##SizeOf()                  \
    {                                                      \
        return sizeof(type);                               \
    }

#define SCRIPT_INSTANCE_CREATE_FUNCTION(Type)                \
    DE_SCRIPT_API Ref<ScriptInstance> CreateInstance##Type() \
    {                                                        \
        return CreateRef<Type>();                            \
    }
