#pragma once

#include "DummyEngine/Core/ECS/System.h"
#include "DummyEngine/Core/ECS/SystemManager.h"

#include <algorithm>  // IWYU pragma: keep

namespace DummyEngine {

#ifdef ECS_IMPLEMENTATION
    namespace {
        using Graph = std::unordered_map<U64, std::unordered_set<U64>>;
        using Order = std::unordered_map<U64, U32>;

        void Traverse(const Graph& g, Order& order, U32& age, U64 node) {
            order[node] = 1;
            for (const auto& node : g.at(node)) {
                auto& current_order = order[node];
                DE_ASSERT(current_order != 1, "Found cycle in dependecy graph");
                if (current_order == 0) {
                    Traverse(g, order, age, node);
                }
            }
            order[node] = age++;
        }

        std::vector<U64> TopologicalSort(const Graph& g) {
            std::vector<U64> result;
            result.reserve(g.size());

            Order order;
            U32   age = 2;
            for (const auto& [id, _] : g) {
                result.emplace_back(id);
                if (order[id] != 0) {
                    Traverse(g, order, age, id);
                }
            }
            std::sort(result.begin(), result.end(), [&](const auto& a, const auto& b) { return order.at(a) < order.at(b); });
            return result;
        }
    }  // namespace

    SystemManager::SystemManager(Storage* storage) {
        storage_ = storage;
    }
    void SystemManager::Update(float dt) {
        for (auto id : cached_order_) {
            systems_[id]->Update(dt);
        }
    }
    void SystemManager::RefreshOrder() {
        cached_order_ = TopologicalSort(dependency_graph_);
    }

#endif

    template <typename Before, typename After> void SystemManager::AddDependency() {
        auto before_index = std::type_index(typeid(Before));
        auto before_it    = system_ids_.find(before_index);
        DE_ASSERT(before_it != system_ids_.end(), "No such system");

        auto after_index = std::type_index(typeid(After));
        auto after_it    = system_ids_.find(after_index);
        DE_ASSERT(after_it != system_ids_.end(), "No such system");

        dependency_graph_[after_it->second].emplace(before_it->second);
        RefreshOrder();
    }
    template <typename SystemType> void SystemManager::AttachSystem(std::shared_ptr<SystemType> system) {
        auto index          = std::type_index(typeid(SystemType));
        auto [it, emplaced] = system_ids_.emplace(index, system_counter_++);
        DE_ASSERT(emplaced, "System already exists");
        systems_[it->second] = system;
        system->Bind(storage_);
        dependency_graph_.emplace(it->second, std::unordered_set<U64>{});
        RefreshOrder();
    }
    template <typename SystemType> void SystemManager::DettachSystem() {
        auto index = std::type_index(typeid(SystemType));
        auto it    = system_ids_.find(index);
        if (it == system_ids_.end()) {
            return;
        }
        systems_.erase(it->second);

        dependency_graph_.erase(it->second);
        for (auto& [id, node] : dependency_graph_) {
            node.erase(it->second);
        }
        RefreshOrder();
        system_ids_.erase(it);
    }

}  // namespace DummyEngine
