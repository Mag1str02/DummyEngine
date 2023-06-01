#pragma once

namespace DE {
#ifdef ECS_IMPLEMENTATION
    void System::Bind(Storage* storage) {
        m_Storage = storage;
    }
#endif
    template <typename... Components> StorageView<Components...> System::View() {
        return m_Storage->View<Components...>();
    }
}  // namespace DE