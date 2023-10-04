#pragma once

#include "DummyEngine/Core/Threading/MutexMap.hpp"
#include "DummyEngine/Utils/Base.h"

#define INDEX(type) std::type_index(typeid(type))

namespace DE {
    //*___CLASS_DECLARATIONS___________________________________________________________________________________________________________________________________________________________________________________________________
    class Entity;
    class Storage;
    class System;
    class IComponentArray;
    class ComponentManager;
    class EntityManager;
    class SystemManager;
    template <typename... Components> class StorageView;
    template <typename ComponentType> class ComponentArray;

    //*___COMPONENT_MANAGER___________________________________________________________________________________________________________________________________________________________________________________________

    class IComponentArray {
    public:
        virtual ~IComponentArray()                          = default;
        virtual void* AddComponent(U32 id, void* component) = 0;
        virtual void* GetComponent(U32 id)                  = 0;
        virtual bool  HasComponent(U32 id)                  = 0;
        virtual void  RemoveComponent(U32 id)               = 0;
    };
    template <typename ComponentType> class ComponentArray : public IComponentArray {
    public:
        ComponentArray() = default;
        virtual void* AddComponent(U32 id, void* component);
        virtual void* GetComponent(U32 id);
        virtual bool  HasComponent(U32 id);
        virtual void  RemoveComponent(U32 id);

    private:
        std::unordered_map<U32, U32> m_EntityToIndex;
        std::vector<U32>             m_IndexToEntity;
        std::vector<ComponentType>   m_ComponentArray;
    };

    class Signature {
    public:
        bool   Get(U64 id) const;
        void   Set(U64 id, bool value);
        bool   Matches(const Signature& required) const;
        size_t Size() const {
            size_t size = 0;
            for (size_t i = 0; i < 64 * m_Data.size(); ++i) {
                if (Get(i)) {
                    ++size;
                }
            }
            return size;
        }

    private:
        std::vector<U64> m_Data;
    };
    class ComponentManager {
    public:
        ComponentManager(Storage* storage);

        template <typename ComponentType> ComponentType* AddComponent(U32 entity_id, const ComponentType& component);
        template <typename ComponentType> ComponentType* GetComponent(U32 entity_id);
        template <typename ComponentType> void           RemoveComponent(U32 entity_id);
        template <typename ComponentType> bool           HasComponent(U32 entity_id);
        void                                             Destroy(U32 entity_id);

        template <typename ComponentType> void SetAddHandler(std::function<void(Entity)> func);
        template <typename ComponentType> void SetRemoveHandler(std::function<void(Entity)> func);

        template <typename... Components> Signature BuildSignature();
        bool                                        Matches(U32 id, const Signature& signature) const;

    private:
        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, bool>::type ValidateComponents();
        template <typename T, typename... Components> bool                                                ValidateComponents();

        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type GetSignature();
        template <typename T, typename... Components> Signature                                                GetSignature();

        void                                   ValidateSignature(U32 entity_id);
        template <typename ComponentType> void RegisterComponent();

        MutexMap<std::type_index>                                             m_ComponentMutex;
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays;
        std::unordered_map<std::type_index, U32>                              m_ComponentId;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      m_AddHandlers;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      m_RemoveHandlers;
        std::vector<Signature>                                                m_Signatures;
        Storage*                                                              m_Storage;
    };

    //*___ENTITY_MANAGER___________________________________________________________________________________________________________________________________________________________________________________________

    class EntityManager {
    public:
        EntityManager() = default;

        std::pair<U32, U32> CreateEntity();
        void                Destroy(U32 id);
        bool                Valid(U32 id, U32 gen) const;

        U32 Generation(U32 id) const;
        U32 NextEntity(U32 id) const;
        U32 BeginEntity() const;
        U32 EndEntity() const;

    private:
        std::vector<U32>  m_Generations;
        std::vector<bool> m_States;
        std::queue<U32>   m_AvailableEntities;
    };

    //*___SYSTEM_MANAGER___________________________________________________________________________________________________________________________________________________________________________________________

    class System {
    public:
        virtual ~System()                    = default;
        virtual void        Update(float dt) = 0;
        virtual std::string GetName() const  = 0;

    protected:
        template <typename... Components> StorageView<Components...> View();

    private:
        friend class SystemManager;
        void Bind(Storage* storage);

        Storage* m_Storage;
    };
    class SystemManager {
    public:
        SystemManager(Storage* storage);
        void                                            Update(float dt);
        template <typename Before, typename After> void AddDependency();
        template <typename SystemType> void             AttachSystem(std::shared_ptr<System> system);

    private:
        std::vector<std::shared_ptr<System>>     m_Systems;
        std::unordered_map<std::type_index, U32> m_SystemId;
        std::vector<std::vector<U32>>            m_DependencyGraph;
        Storage*                                 m_Storage;
    };

    //*___ENTITY____________________________________________________________________________________________________________________________________________________________________________________________________

    class Entity {
    public:
        Entity();
        ~Entity()                        = default;
        Entity(const Entity&)            = default;
        Entity(Entity&&)                 = default;
        Entity& operator=(const Entity&) = default;
        Entity& operator=(Entity&&)      = default;

        bool Valid() const;
        void Destroy();

        template <typename ComponentType> ComponentType* AddComponent(ComponentType component = ComponentType());
        template <typename ComponentType> ComponentType* GetComponent();
        template <typename ComponentType> ComponentType& Add(ComponentType component = ComponentType());
        template <typename ComponentType> ComponentType& Get();
        template <typename ComponentType> bool           Has() const;
        template <typename ComponentType> void           Remove();

        bool operator==(const Entity& other) const;
        bool operator!=(const Entity& other) const;

    private:
        template <typename... Components> friend class StorageView;
        friend struct std::hash<Entity>;
        friend class Storage;

        U32              m_ID;
        U32              m_Gen;
        WeakRef<Storage> m_Storage;
    };

    //*___STORAGE___________________________________________________________________________________________________________________________________________________________________________________________

    class Storage : public std::enable_shared_from_this<Storage> {
    public:
        Storage();
        // If any of remove callbacks use functions of passed entity, Destruct should be called before destructor
        void Destruct();
        ~Storage()                         = default;
        Storage(const Storage&)            = delete;
        Storage(Storage&&)                 = delete;
        Storage& operator=(const Storage&) = delete;
        Storage& operator=(Storage&&)      = delete;

        Entity                                                       CreateEntity();
        template <typename... Components> StorageView<Components...> View();

        template <typename ComponentType> void SetAddHandler(std::function<void(Entity)> func);
        template <typename ComponentType> void SetRemoveHandler(std::function<void(Entity)> func);

        template <typename SystemType> void AttachSystem(std::shared_ptr<System> system);
        void                                UpdateSystems(float dt);

    private:
        template <typename... Components> friend class StorageView;
        friend class Entity;
        friend class ComponentManager;
        friend struct StorageDeleter;

        Entity GetEntity(U32 id);
        bool   Valid(U32 id, U32 gen) const;
        void   Destroy(U32 id, U32 gen);

        template <typename ComponentType> ComponentType* AddComponent(U32 id, U32 gen, const ComponentType& component);
        template <typename ComponentType> ComponentType* GetComponent(U32 id, U32 gen);
        template <typename ComponentType> bool           HasComponent(U32 id, U32 gen);
        template <typename ComponentType> void           RemoveComponent(U32 id, U32 gen);

        EntityManager    m_EntityManager;
        ComponentManager m_ComponentManager;
        SystemManager    m_SystemManager;
    };
    template <typename... Components> class StorageView {
    public:
        class Iterator {
        public:
            Iterator(U32 id, StorageView* v);

            bool      operator==(const Iterator& other) const;
            bool      operator!=(const Iterator& other) const;
            Iterator& operator++();
            Iterator  operator++(int);
            Entity    operator*();

        private:
            friend class StorageView;
            U32          m_ID;
            StorageView* m_View;
        };
        Iterator begin();
        Iterator end();
        bool     Empty();

    private:
        friend class Storage;
        StorageView(Storage* storage);

        Signature m_Signature;
        Storage*  m_Storage;
    };

}  // namespace DE

#include "DummyEngine/Core/ECS/ComponentArray.hpp"
#include "DummyEngine/Core/ECS/ComponentManager.hpp"
#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/ECS/EntityManager.hpp"
#include "DummyEngine/Core/ECS/Storage.hpp"
#include "DummyEngine/Core/ECS/StorageView.hpp"
#include "DummyEngine/Core/ECS/System.hpp"
#include "DummyEngine/Core/ECS/SystemManager.hpp"