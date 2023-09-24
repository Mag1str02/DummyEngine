#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    template <class T> class HandleManager;
    template <class T> class Handle {
    public:
        T&       operator*();
        const T& operator*() const;
        T*       operator->();
        const T* operator->() const;
        bool     Valid() const;
        void     Destroy();
        //! Temporary
        U32 GetId() const { return m_ID; }

    private:
        friend class HandleManager<T>;
        HandleManager<T>* m_Manager = nullptr;
        U32               m_ID      = 0;
        U32               m_Gen     = 0;
    };

    template <class T> class HandleManager {
    public:
        class Iterator {
        public:
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

            Iterator& operator++();
            Iterator  operator++(int);
            Iterator& operator--();
            Iterator  operator--(int);
            T&        operator*();
            T*        operator->();
            U32       ID() const;

        private:
            friend class HandleManager;
            Iterator(HandleManager<T>* manager, U32 id);

            HandleManager<T>* m_Manager = nullptr;
            U32               m_ID      = 0;
        };

        Iterator begin();
        Iterator end();

        Handle<T> CreateHandle();
        void      Destroy(U32 id);
        void      Clear();

    private:
        friend class Handle<T>;
        void GenId();

        struct Instance {
            T    instance;
            U32  gen   = 0;
            bool valid = false;
        };

        std::deque<U32>       m_AvailableIds;
        std::vector<Instance> m_Instances;
    };

    template <class T> bool HandleManager<T>::Iterator::operator==(const Iterator& other) const {
        return m_Manager == other.m_Manager && m_ID == other.m_ID;
    }
    template <class T> bool HandleManager<T>::Iterator::operator!=(const Iterator& other) const {
        return m_Manager != other.m_Manager || m_ID != other.m_ID;
    }

    template <class T> typename HandleManager<T>::Iterator& HandleManager<T>::Iterator::operator++() {
        do {
            ++m_ID;
        } while (m_ID < m_Manager->m_Instances.size() && !m_Manager->m_Instances[m_ID].valid);
        return *this;
    }
    template <class T> typename HandleManager<T>::Iterator HandleManager<T>::Iterator::operator++(int) {
        Iterator res = *this;
        do {
            ++m_ID;
        } while (m_ID < m_Manager->m_Instances.size() && !m_Manager->m_Instances[m_ID].valid);
        return res;
    }
    template <class T> typename HandleManager<T>::Iterator& HandleManager<T>::Iterator::operator--() {
        do {
            --m_ID;
        } while (m_ID > 0 && !m_Manager->m_Instances[m_ID].valid);
        return *this;
    }
    template <class T> typename HandleManager<T>::Iterator HandleManager<T>::Iterator::operator--(int) {
        Iterator res = *this;
        do {
            --m_ID;
        } while (m_ID > 0 && !m_Manager->m_Instances[m_ID].valid);
        return res;
    }
    template <class T> T& HandleManager<T>::Iterator::operator*() {
        return m_Manager->m_Instances[m_ID].instance;
    }
    template <class T> T* HandleManager<T>::Iterator::operator->() {
        return &(m_Manager->m_Instances[m_ID].instance);
    }
    template <class T> U32 HandleManager<T>::Iterator::ID() const {
        return m_ID;
    }

    template <class T> HandleManager<T>::Iterator::Iterator(HandleManager<T>* manager, U32 id) : m_Manager(manager), m_ID(id) {}

    template <class T> typename HandleManager<T>::Iterator HandleManager<T>::begin() {
        for (size_t i = 0; i < m_Instances.size(); ++i) {
            if (m_Instances[i].valid) {
                return Iterator(this, i);
            }
        }
        return Iterator(this, m_Instances.size());
    }
    template <class T> typename HandleManager<T>::Iterator HandleManager<T>::end() {
        return Iterator(this, m_Instances.size());
    }

    template <class T> Handle<T> HandleManager<T>::CreateHandle() {
        if (m_AvailableIds.empty()) {
            GenId();
        }
        Handle<T> res;
        res.m_Manager               = this;
        res.m_ID                    = m_AvailableIds.front();
        res.m_Gen                   = ++m_Instances[res.m_ID].gen;
        m_Instances[res.m_ID].valid = true;
        m_AvailableIds.pop_front();
        return res;
    }
    template <class T> void HandleManager<T>::Destroy(U32 id) {
        if (m_Instances[id].valid) {
            m_Instances[id].valid    = false;
            m_Instances[id].instance = T();
            m_AvailableIds.push_back(id);
        }
    }
    template <class T> void HandleManager<T>::Clear() {
        for (U32 i = 0; i < m_Instances.size(); ++i) {
            Destroy(i);
        }
    }
    template <class T> void HandleManager<T>::GenId() {
        m_AvailableIds.push_back(m_Instances.size());
        m_Instances.push_back(Instance());
    }

    template <class T> T& Handle<T>::operator*() {
        return m_Manager->m_Instances[m_ID].instance;
    }
    template <class T> const T& Handle<T>::operator*() const {
        return m_Manager->m_Instances[m_ID].instance;
    }
    template <class T> T* Handle<T>::operator->() {
        return &(m_Manager->m_Instances[m_ID].instance);
    }
    template <class T> const T* Handle<T>::operator->() const {
        return &(m_Manager->m_Instances[m_ID].instance);
    }
    template <class T> bool Handle<T>::Valid() const {
        return m_Manager && m_Manager->m_Instances[m_ID].valid && m_Manager->m_Instances[m_ID].gen == m_Gen;
    }
    template <class T> void Handle<T>::Destroy() {
        m_Manager->m_AvailableIds.push_back(m_ID);
        m_Manager->m_Instances[m_ID].valid = false;
    }
}  // namespace DE
