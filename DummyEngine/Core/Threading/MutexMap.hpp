#pragma once

#include "DummyEngine/Utils/Base.h"

template <typename K>
class MutexMap {
public:
    std::mutex& get(const K& key) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_map[key];
    }
private:
    std::mutex m_mutex;
    std::unordered_map<K, std::mutex> m_map;
};