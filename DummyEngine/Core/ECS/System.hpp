#pragma once

namespace DummyEngine {
#ifdef ECS_IMPLEMENTATION
    void System::Bind(Storage* storage) {
        storage_ = storage;
    }
#endif
    template <typename... Components> StorageView<Components...> System::View() {
        return storage_->View<Components...>();
    }
}  // namespace DummyEngine