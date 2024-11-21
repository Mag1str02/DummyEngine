#pragma once

#include "DummyEngine/Utils/Debug/Assert.h"  // IWYU pragma: export
#include "DummyEngine/Utils/Types/Types.h"

#include <typeindex>

#define INDEX(type) std::type_index(typeid(type))

namespace DummyEngine {

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
        std::unordered_map<U32, U32> entity_to_index_;
        std::vector<U32>             index_to_entity_;
        std::vector<ComponentType>   component_array_;
    };

    class Signature {
    public:
        bool   Get(U64 id) const;
        void   Set(U64 id, bool value);
        bool   Matches(const Signature& required) const;
        size_t Size() const {
            size_t size = 0;
            for (size_t i = 0; i < 64 * data_.size(); ++i) {
                if (Get(i)) {
                    ++size;
                }
            }
            return size;
        }

    private:
        std::vector<U64> data_;
    };
    class ComponentManager {
    public:
        explicit ComponentManager(Storage* storage);

        template <typename ComponentType> ComponentType* AddComponent(U32 entity_id, const ComponentType& component);
        template <typename ComponentType> ComponentType* GetComponent(U32 entity_id);
        template <typename ComponentType> void           RemoveComponent(U32 entity_id);
        template <typename ComponentType> bool           HasComponent(U32 entity_id) const;
        void                                             Destroy(U32 entity_id);

        template <typename ComponentType> void SetAddHandler(std::function<void(Entity)> func);
        template <typename ComponentType> void SetRemoveHandler(std::function<void(Entity)> func);

        template <typename... Components> Signature BuildSignature();
        bool                                        Matches(U32 id, const Signature& signature) const;

    private:
        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, bool>::type ValidateComponents() const;
        template <typename T, typename... Components> bool                                                ValidateComponents() const;

        template <typename... Components> typename std::enable_if<sizeof...(Components) == 0, Signature>::type GetSignature() const;
        template <typename T, typename... Components> Signature                                                GetSignature() const;

        void                                   ValidateSignature(U32 entity_id);
        template <typename ComponentType> void RegisterComponent();

        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> component_arrays_;
        std::unordered_map<std::type_index, U32>                              component_id_;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      add_handlers_;
        std::unordered_map<std::type_index, std::function<void(Entity)>>      remove_handlers_;
        std::vector<Signature>                                                signatures_;
        Storage*                                                              storage_;
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
        std::vector<U32>  generations_;
        std::vector<bool> states_;
        std::queue<U32>   available_entities_;
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

        Storage* storage_;
    };
    class SystemManager {
    public:
        explicit SystemManager(Storage* storage);
        void                                            Update(float dt);
        template <typename Before, typename After> void AddDependency();
        template <typename SystemType> void             AttachSystem(std::shared_ptr<System> system);

    private:
        std::vector<std::shared_ptr<System>>     systems_;
        std::unordered_map<std::type_index, U32> system_ids_;
        std::vector<std::vector<U32>>            dependency_graph_;
        Storage*                                 storage_;
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

        U32              id_;
        U32              gen_;
        WeakRef<Storage> storage_;
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
        template <typename ComponentType> bool           HasComponent(U32 id, U32 gen) const;
        template <typename ComponentType> void           RemoveComponent(U32 id, U32 gen);

        EntityManager    entity_manager_;
        ComponentManager component_manager_;
        SystemManager    system_manager_;
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
            U32          id_;
            StorageView* view_;
        };
        Iterator begin();  // NOLINT
        Iterator end();    // NOLINT
        bool     Empty();

    private:
        friend class Storage;
        explicit StorageView(Storage* storage);

        Signature signature_;
        Storage*  storage_;
    };

}  // namespace DummyEngine

#include "DummyEngine/Core/ECS/ComponentArray.hpp"    // IWYU pragma: export
#include "DummyEngine/Core/ECS/ComponentManager.hpp"  // IWYU pragma: export
#include "DummyEngine/Core/ECS/Entity.hpp"            // IWYU pragma: export
#include "DummyEngine/Core/ECS/EntityManager.hpp"     // IWYU pragma: export
#include "DummyEngine/Core/ECS/Storage.hpp"           // IWYU pragma: export
#include "DummyEngine/Core/ECS/StorageView.hpp"       // IWYU pragma: export
#include "DummyEngine/Core/ECS/System.hpp"            // IWYU pragma: export
#include "DummyEngine/Core/ECS/SystemManager.hpp"     // IWYU pragma: export