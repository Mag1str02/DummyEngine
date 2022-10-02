#include "Addition/Base.h"
#include "Core/ECS/Entity.hpp"

namespace DE
{
    class ScriptInstance
    {
    public:
        ScriptInstance() : m_Scene(nullptr) {}
        virtual ~ScriptInstance() {}

        virtual void OnAttach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnDetach() {}

        ScriptInstance(const ScriptInstance&) = delete;
        ScriptInstance(ScriptInstance&&)      = delete;
        ScriptInstance& operator=(const ScriptInstance&) = delete;
        ScriptInstance& operator=(ScriptInstance&&) = delete;

    protected:
        template <typename T> T& GetComponent() { return m_Entity.GetComponent<T>(); }
        Entity                   GetEntityByName(const std::string& name) const { return m_Scene->GetByName(name); };

    private:
        friend class Scene;

        void AttachToEntity(Scene* scene, Entity entity)
        {
            m_Scene  = scene;
            m_Entity = entity;
        }

        Scene* m_Scene;
        Entity m_Entity;
    };
}  // namespace DE

// clang-format off
#define SCRIPT_INSTANCE_CREATE_FUNCTION(Type) DE_SCRIPT_API Ref<ScriptInstance> CreateInstance(){return CreateRef<Type>();}
// clang-format on