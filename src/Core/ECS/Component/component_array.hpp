#pragma once

#include <cinttypes>
#include <iostream>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Addition/some_funcs.h"
#include "../../Addition/types.h"
#include "../../Initializer/initializer.h"

namespace DE
{

    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(EntityId id) = 0;
        virtual std::string LogState() const
        {
            return "";
        }
    };

    template <typename ComponentType>
    class ComponentArray : public IComponentArray
    {
    private:
        friend class ComponentManager;
        std::vector<ComponentType> _component_array;
        std::unordered_map<EntityId, size_t> _entity_to_index;
        std::unordered_map<size_t, EntityId> _index_to_entity;

        uint64_t _entity_amount;

        void InsertComponent(EntityId id, const ComponentType &component)
        {
            if (_entity_to_index.find(id) != _entity_to_index.end())
            {
                _component_array[_entity_to_index[id]] = component;
                Logger::Info("ECS", "ComponentArray",
                             "Changed existing component (" + NormalTypeName(typeid(ComponentType).name()) + ") of Entity (" + std::to_string(id) + ")");
            }
            else
            {
                _entity_to_index[id] = _entity_amount;
                _component_array[_entity_amount] = component;
                _index_to_entity[_entity_amount] = id;
                ++_entity_amount;
                Logger::Info("ECS", "ComponentArray",
                             "Added component (" + NormalTypeName(typeid(ComponentType).name()) + ") to Entity (" + std::to_string(id) + ")");
            }
        }
        void RemoveComponent(EntityId id)
        {
            if (_entity_to_index.find(id) != _entity_to_index.end())
            {
                size_t current_component_index = _entity_to_index[id];
                size_t last_component_index = _entity_amount - 1;
                EntityId last_entity_id = _index_to_entity[last_component_index];

                _component_array[current_component_index] = _component_array[last_component_index];
                _entity_to_index[last_entity_id] = current_component_index;
                _index_to_entity[current_component_index] = last_entity_id;
                _index_to_entity.erase(last_component_index);
                _entity_to_index.erase(id);
                --_entity_amount;
                Logger::Info("ECS", "ComponentArray",
                             "Removed component " + NormalTypeName(typeid(ComponentType).name()) + " from Entity " + std::to_string(id));
            }
        }
        void EntityDestroyed(EntityId id) override
        {
            RemoveComponent(id);
        }

    public:
        ComponentArray()
        {
            _entity_amount = 0;
            _component_array.resize(Initializer::Get()._max_entity_amount);
        }
        ComponentType &GetComponent(EntityId id)
        {
            return _component_array[_entity_to_index[id]];
        }

        std::string LogState() const override
        {
            std::string log_state = "";
            log_state.append("Entity to Index:\n");
            for (const auto &[entity, index] : _entity_to_index)
            {
                log_state.append("  " + std::to_string(entity) + "->" + std::to_string(index) + "\n");
            }
            log_state.append("Index to Entity:\n");
            for (const auto &[index, entity] : _index_to_entity)
            {
                log_state.append("  " + std::to_string(index) + "->" + std::to_string(entity) + "\n");
            }
            return log_state;
        }
    };
} // namespace DE
