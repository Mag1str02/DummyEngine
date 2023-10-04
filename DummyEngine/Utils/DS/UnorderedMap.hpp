#pragma once

#include "../Base/STDIncludes.h"

namespace DE {

    // Thread safe unordered map
    template <typename K, typename V> class UnorderedMap {
    public:
        UnorderedMap() = default;
        const V& operator[](const K& key);
        void     insert(const K& key, const V& value);
        bool     contains(const K& key);
        void     clear();
        void     erase(const K& key);

    private:
        std::unordered_map<K, V> m_map;
        std::mutex               m_mutex;
    };

}  // namespace DE