#pragma once

#include <functional>
#include <unordered_map>
#include <memory>
#include "UIWidgetComponent.h"

namespace DummyEngine {

enum class UIEventType {
    MouseEnter,
    MouseLeave,
    MouseClick,
    MousePress,
    MouseRelease,
    TextInput
};

class UIEvent {
public:
    UIEvent(UIEventType type, std::shared_ptr<UIWidgetComponent> source)
        : m_type(type), m_source(source) {}

    UIEventType GetType() const { return m_type; }
    std::shared_ptr<UIWidgetComponent> GetSource() const { return m_source; }

private:
    UIEventType m_type;
    std::shared_ptr<UIWidgetComponent> m_source;
};

class UIEventSystem {
public:
    using EventCallback = std::function<void(const UIEvent&)>;

    static UIEventSystem& GetInstance();

    void RegisterCallback(UIEventType type, EventCallback callback);
    void UnregisterCallback(UIEventType type);
    void DispatchEvent(const UIEvent& event);

private:
    UIEventSystem() = default;
    ~UIEventSystem() = default;

    std::unordered_map<UIEventType, std::vector<EventCallback>> m_callbacks;
};

} // namespace DummyEngine 