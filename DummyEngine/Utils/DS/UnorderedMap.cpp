#pragma once
#include "UnorderedMap.hpp"

namespace DE {
    template <typename K, typename V> const V& UnorderedMap<K, V>::operator[](const K& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_map[key];
    }

    template <typename K, typename V> void UnorderedMap<K, V>::insert(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_map.insert({key, value});
    }

    template <typename K, typename V> void UnorderedMap<K, V>::erase(const K& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_map.erase(key);
    }

    template <typename K, typename V> void UnorderedMap<K, V>::clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_map.clear();
    }

    template <typename K, typename V> bool UnorderedMap<K, V>::contains(const K& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_map.contains(key);
    }

}  // namespace DE