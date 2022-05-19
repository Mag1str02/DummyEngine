#pragma once

#include <cinttypes>

#include "../../../Addition/some_funcs.h"
#include "../../../Addition/types.h"
#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../Initializer/initializer.h"

namespace DE {

class IComponentArray {
protected:
    friend class ComponentManager;

    virtual void EntityDestroyed(EntityId entity_id) = 0;
    virtual void ExtendArray() = 0;

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

    ComponentArray(size_t entity_amount) {
        _entity_to_index.assign(entity_amount, -1);
    }

    std::string LogState() const override {
        std::string log_state = "";
        log_state.append("Entity to Index:\n");
        for (size_t i = 0; i < _entity_to_index.size(); ++i) {
            if (_entity_to_index[i] != -1) {
                log_state.append("  " + std::to_string(i) + "->" + std::to_string(_entity_to_index[i]) + "\n");
            }
        }
        log_state.append("Index to Entity:\n");
        for (size_t i = 0; i < _index_to_entity.size(); ++i) {
            log_state.append("  " + std::to_string(i) + "->" + std::to_string(_index_to_entity[i]) + "\n");
        }
        return log_state;
    }

    bool HasComponent(EntityId entity_id) {
        return _entity_to_index[entity_id] != -1;
    }
    ComponentType& operator[](EntityId entity_id) {
        return _component_array.at(_entity_to_index[entity_id]);
    }

    iterator begin() {
        return iterator(0, this);
    }
    iterator end() {
        return iterator(_component_array.size(), this);
    }

private:
    friend class ComponentManager;

    std::vector<ComponentType> _component_array;
    std::vector<int64_t> _entity_to_index;
    std::vector<EntityId> _index_to_entity;

    void ExtendArray() override {
        _entity_to_index.push_back(-1);
    }

    void InsertComponent(EntityId entity_id, const ComponentType& component) {
        if (_entity_to_index[entity_id] != -1) {
            _component_array[_entity_to_index[entity_id]] = component;

            Logger::Info("ECS", "ComponentArray", "Changed existing component (" + NormalTypeName(typeid(ComponentType).name()) + ") of Entity (" + std::to_string(entity_id) + ")");
        } else {
            _entity_to_index[entity_id] = _component_array.size();
            _component_array.push_back(component);
            _index_to_entity.push_back(entity_id);

            Logger::Info("ECS", "ComponentArray", "Added component (" + NormalTypeName(typeid(ComponentType).name()) + ") to Entity (" + std::to_string(entity_id) + ")");
        }
    }
    void RemoveComponent(EntityId entity_id) {
        if (_entity_to_index[entity_id] != -1) {
            size_t current_component_index = _entity_to_index[entity_id];
            size_t last_component_index = _component_array.size() - 1;
            EntityId last_entity_id = _index_to_entity[last_component_index];

            _component_array[current_component_index] = _component_array[last_component_index];
            _component_array.pop_back();
            _entity_to_index[last_entity_id] = current_component_index;
            _index_to_entity[current_component_index] = last_entity_id;
            _index_to_entity.pop_back();
            _entity_to_index[entity_id] = -1;

            Logger::Info("ECS", "ComponentArray", "Removed component " + NormalTypeName(typeid(ComponentType).name()) + " from Entity " + std::to_string(entity_id));
        }
    }
    void EntityDestroyed(EntityId entity_id) override {
        RemoveComponent(entity_id);
    }
};
}  // namespace DE
