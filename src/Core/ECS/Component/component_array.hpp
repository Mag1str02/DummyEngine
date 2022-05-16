#pragma once

#include <cinttypes>

#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Addition/some_funcs.h"
#include "../../Addition/types.h"
#include "../../Initializer/initializer.h"

namespace DE {

class IComponentArray {
protected:
    friend class ComponentManager;

    virtual void EntityDestroyed(EntityId entity_id) = 0;

public:
    virtual ~IComponentArray() = default;
    virtual std::string LogState() const = 0;
};

template <typename ComponentType>
class ComponentArray : public IComponentArray {
public:
    class iterator {
    private:
        size_t _index = 0;
        ComponentArray<ComponentType>* _owner;

    public:
        iterator(size_t index, ComponentArray<ComponentType>* owner) : _index(index), _owner(owner) {
        }

        std::pair<EntityId, ComponentType&> operator*() {
            return {(*_owner)._index_to_entity[_index], (*_owner)._component_array[_index]};
        }
        EntityId entity_id() {
            return (*_owner)._index_to_entity.at(_index);
        }
        ComponentType& component() {
            return (*_owner)._component_array.at(_index);
        }

        iterator& operator++() {
            ++_index;
            return *this;
        }
        iterator operator++(int) {
            iterator res(_index);
            ++_index;
            return res;
        }
        iterator& operator--() {
            --_index;
            return *this;
        }
        iterator operator--(int) {
            iterator res(_index);
            --_index;
            return res;
        }

        bool operator==(const iterator& other) const {
            return _index == other._index;
        }
        bool operator!=(const iterator& other) const {
            return _index != other._index;
        }
    };

    ComponentArray() {
        _entity_amount = 0;
        _component_array.resize(Initializer::Get()._max_entity_amount);
    }

    std::string LogState() const override {
        std::string log_state = "";
        log_state.append("Entity to Index:\n");
        for (const auto& [entity, index] : _entity_to_index) {
            log_state.append("  " + std::to_string(entity) + "->" + std::to_string(index) + "\n");
        }
        log_state.append("Index to Entity:\n");
        for (const auto& [index, entity] : _index_to_entity) {
            log_state.append("  " + std::to_string(index) + "->" + std::to_string(entity) + "\n");
        }
        return log_state;
    }

    EntityId Owner(iterator it) {
        return _index_to_entity.at(it._index);
    }
    bool HasComponent(EntityId entity_id) {
        return _entity_to_index.find(entity_id) != _entity_to_index.end();
    }
    ComponentType& operator[](EntityId entity_id) {
        return _component_array[_entity_to_index.at(entity_id)];
    }

    iterator begin() {
        return iterator(0, this);
    }
    iterator end() {
        return iterator(_entity_amount, this);
    }

private:
    friend class ComponentManager;

    std::vector<ComponentType> _component_array;
    std::unordered_map<EntityId, size_t> _entity_to_index;
    std::unordered_map<size_t, EntityId> _index_to_entity;
    uint64_t _entity_amount;

    void InsertComponent(EntityId entity_id, const ComponentType& component) {
        if (_entity_to_index.find(entity_id) != _entity_to_index.end()) {
            _component_array[_entity_to_index[entity_id]] = component;

            Logger::Info(
                "ECS", "ComponentArray",
                "Changed existing component (" + NormalTypeName(typeid(ComponentType).name()) + ") of Entity (" + std::to_string(entity_id) + ")");
        } else {
            _entity_to_index[entity_id] = _entity_amount;
            _component_array[_entity_amount] = component;
            _index_to_entity[_entity_amount] = entity_id;
            ++_entity_amount;

            Logger::Info("ECS", "ComponentArray",
                         "Added component (" + NormalTypeName(typeid(ComponentType).name()) + ") to Entity (" + std::to_string(entity_id) + ")");
        }
    }
    void RemoveComponent(EntityId entity_id) {
        if (_entity_to_index.find(entity_id) != _entity_to_index.end()) {
            size_t current_component_index = _entity_to_index[entity_id];
            size_t last_component_index = _entity_amount - 1;
            EntityId last_entity_id = _index_to_entity[last_component_index];

            _component_array[current_component_index] = _component_array[last_component_index];
            _entity_to_index[last_entity_id] = current_component_index;
            _index_to_entity[current_component_index] = last_entity_id;
            _index_to_entity.erase(last_component_index);
            _entity_to_index.erase(entity_id);
            --_entity_amount;

            Logger::Info("ECS", "ComponentArray",
                         "Removed component " + NormalTypeName(typeid(ComponentType).name()) + " from Entity " + std::to_string(entity_id));
        }
    }
    void EntityDestroyed(EntityId entity_id) override {
        RemoveComponent(entity_id);
    }
};
}  // namespace DE
