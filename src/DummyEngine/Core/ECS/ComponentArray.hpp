#pragma once

#include "Addition/Base.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{

    class IComponentArray
    {
    protected:
        friend class ComponentManager;

        virtual void EntityDestroyed(EntityId entity_id) = 0;
        virtual void ExtendArray() = 0;

    public:
        virtual ~IComponentArray() = default;
        virtual std::string LogState() const = 0;
    };

    template <typename ComponentType>
    class ComponentArray : public IComponentArray
    {
    public:
        class iterator
        {
        private:
            size_t m_Index = 0;
            ComponentArray<ComponentType>* m_Owner;

        public:
            iterator(size_t index, ComponentArray<ComponentType>* owner) : m_Index(index), m_Owner(owner) {}

            std::pair<EntityId, ComponentType&> operator*()
            {
                return {(*m_Owner).m_IndexToEntity[m_Index], (*m_Owner).m_ComponentArray[m_Index]};
            }
            EntityId entity_id()
            {
                return (*m_Owner).m_IndexToEntity.at(m_Index);
            }
            ComponentType& component()
            {
                return (*m_Owner).m_ComponentArray.at(m_Index);
            }

            iterator& operator++()
            {
                ++m_Index;
                return *this;
            }
            iterator operator++(int)
            {
                iterator res(m_Index);
                ++m_Index;
                return res;
            }
            iterator& operator--()
            {
                --m_Index;
                return *this;
            }
            iterator operator--(int)
            {
                iterator res(m_Index);
                --m_Index;
                return res;
            }

            bool operator==(const iterator& other) const
            {
                return m_Index == other.m_Index;
            }
            bool operator!=(const iterator& other) const
            {
                return m_Index != other.m_Index;
            }
        };

        ComponentArray(size_t entity_amount)
        {
            m_EntityAmount.assign(entity_amount, -1);
        }

        std::string LogState() const override
        {
            std::string log_state = "";
            log_state.append("Entity to Index:\n");
            for (size_t i = 0; i < m_EntityAmount.size(); ++i)
            {
                if (m_EntityAmount[i] != -1)
                {
                    log_state.append("  " + std::to_string(i) + "->" + std::to_string(m_EntityAmount[i]) + "\n");
                }
            }
            log_state.append("Index to Entity:\n");
            for (size_t i = 0; i < m_IndexToEntity.size(); ++i)
            {
                log_state.append("  " + std::to_string(i) + "->" + std::to_string(m_IndexToEntity[i]) + "\n");
            }
            return log_state;
        }

        bool HasComponent(EntityId entity_id)
        {
            return m_EntityAmount[entity_id] != -1;
        }
        ComponentType& operator[](EntityId entity_id)
        {
            DE_ASSERT(0 <= entity_id && entity_id < m_EntityAmount.size(), "Wrong enitity id.");
            DE_ASSERT(m_EntityAmount[entity_id] != -1, "Given entity has no such component.");
            return m_ComponentArray.at(m_EntityAmount[entity_id]);
        }

        iterator begin()
        {
            return iterator(0, this);
        }
        iterator end()
        {
            return iterator(m_ComponentArray.size(), this);
        }

    private:
        friend class ComponentManager;

        std::vector<ComponentType> m_ComponentArray;
        std::vector<int64_t> m_EntityAmount;
        std::vector<EntityId> m_IndexToEntity;

        static std::string NormalTypeName(const std::string& s_name)
        {
            for (size_t i = 1; i < s_name.size(); ++i)
            {
                if ('0' > s_name[i] || s_name[i] > '9')
                {
                    return s_name.substr(i, s_name.size() - i);
                }
            }
            return s_name;
        }

        void ExtendArray() override
        {
            m_EntityAmount.push_back(-1);
        }

        void InsertComponent(EntityId entity_id, const ComponentType& component)
        {
            if (m_EntityAmount[entity_id] != -1)
            {
                m_ComponentArray[m_EntityAmount[entity_id]] = component;

                Logger::Info("ECS",
                             "ComponentArray",
                             "Changed existing component (" + NormalTypeName(typeid(ComponentType).name()) +
                                 ") of Entity (" + std::to_string(entity_id) + ")");
            }
            else
            {
                m_EntityAmount[entity_id] = m_ComponentArray.size();
                m_ComponentArray.push_back(component);
                m_IndexToEntity.push_back(entity_id);

                Logger::Info("ECS",
                             "ComponentArray",
                             "Added component (" + NormalTypeName(typeid(ComponentType).name()) + ") to Entity (" +
                                 std::to_string(entity_id) + ")");
            }
        }
        void RemoveComponent(EntityId entity_id)
        {
            if (m_EntityAmount[entity_id] != -1)
            {
                size_t current_component_index = m_EntityAmount[entity_id];
                size_t last_component_index = m_ComponentArray.size() - 1;
                EntityId last_entity_id = m_IndexToEntity[last_component_index];

                m_ComponentArray[current_component_index] = m_ComponentArray[last_component_index];
                m_ComponentArray.pop_back();
                m_EntityAmount[last_entity_id] = current_component_index;
                m_IndexToEntity[current_component_index] = last_entity_id;
                m_IndexToEntity.pop_back();
                m_EntityAmount[entity_id] = -1;

                Logger::Info("ECS",
                             "ComponentArray",
                             "Removed component " + NormalTypeName(typeid(ComponentType).name()) + " from Entity " +
                                 std::to_string(entity_id));
            }
        }
        void EntityDestroyed(EntityId entity_id) override
        {
            RemoveComponent(entity_id);
        }
    };
}  // namespace DE
