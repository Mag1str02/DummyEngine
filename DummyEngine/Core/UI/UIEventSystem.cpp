#include "UIEventSystem.h"

namespace DummyEngine {

UIEventSystem& UIEventSystem::GetInstance() {
    static UIEventSystem instance;
    return instance;
}

void UIEventSystem::RegisterCallback(UIEventType type, EventCallback callback) {
    m_callbacks[type].push_back(callback);
}

void UIEventSystem::UnregisterCallback(UIEventType type) {
    m_callbacks.erase(type);
}

void UIEventSystem::DispatchEvent(const UIEvent& event) {
    auto it = m_callbacks.find(event.GetType());
    if (it != m_callbacks.end()) {
        for (const auto& callback : it->second) {
            callback(event);
        }
    }
}

} // namespace DummyEngine 