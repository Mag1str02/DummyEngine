#pragma once

#include "DummyEngine/Utils/Debug/Assert.h"
#include "DummyEngine/Utils/Types/Types.h"

#include <functional>
#include <variant>
#include <vector>

struct GLFWwindow;
struct GLFWmonitor;

namespace DummyEngine {
    template <typename Event> using EventCallback = std::function<void(const Event&)>;

    enum class EventType {
        None = 0,

        //*Responding

        Monitor,

        WindowCursorEntered,
        WindowFocus,
        WindowResize,
        WindowClose,

        Key,
        Char,

        MouseButton,
        MouseScrolled,
        MousePosition,

        //*Triggering

        SetWindowMode,
        SetMouseLock,

        Count
    };

    class Event;

    template <EventType Type>
    struct EventBase {
        static constexpr EventType kType = Type;

        GLFWwindow* Window;
    };

#define DECLARE_EVENT(name) struct name##Event : EventBase<EventType::name>

    DECLARE_EVENT(WindowCursorEntered) {
        int Entered;
    };
    DECLARE_EVENT(WindowFocus) {
        int Focused;
    };
    DECLARE_EVENT(WindowResize) {
        int Width;
        int Height;
    };
    DECLARE_EVENT(WindowClose){};

    DECLARE_EVENT(Key) {
        int Key;
        int Scancode;
        int Action;
        int Mods;
    };
    DECLARE_EVENT(Char) {
        unsigned int C;
    };

    DECLARE_EVENT(MouseButton) {
        int Button;
        int Action;
        int Mods;
    };
    DECLARE_EVENT(MouseScrolled) {
        double XOffset;
        double YOffset;
    };
    DECLARE_EVENT(MousePosition) {
        double X;
        double Y;
    };

    DECLARE_EVENT(Monitor) {
        GLFWmonitor* Monitor;
        int          Event;
    };
    DECLARE_EVENT(SetWindowMode) {
        bool Fullscreen;
        U32  MonitorId;
    };
    DECLARE_EVENT(SetMouseLock) {
        enum State : U32 {
            Lock,
            UnLock,
            Switch,
        } Action;
    };

    class Event {
    private:
        using Value = std::variant<    //
            MonitorEvent,              //
            WindowCursorEnteredEvent,  //
            WindowFocusEvent,          //
            WindowResizeEvent,         //
            WindowCloseEvent,          //
            KeyEvent,                  //
            CharEvent,                 //
            MouseButtonEvent,          //
            MouseScrolledEvent,        //
            MousePositionEvent,        //
            SetWindowModeEvent,        //
            SetMouseLockEvent          //
            >;

    public:
        template <typename T>
        Event(T t) : type_(T::kType), value_(std::move(t)) {}  // NOLINT

        EventType Type() const { return type_; }

        template <typename T>
        const T& As() const {
            DE_ASSERT(type_ == T::kType, "Invalid event type");
            return std::get<T>(value_);
        }

    private:
        EventType type_;
        Value     value_;
    };

    class EventDispatcher {
    public:
        template <typename ListeningEvent> void AddEventListener(EventCallback<ListeningEvent> callback) {
            auto base_callback = [func = std::move(callback)](const Event& e) { func(e.As<ListeningEvent>()); };
            event_callbacks_[(size_t)ListeningEvent::kType].push_back(base_callback);
        }

        void Dispatch(const Event& event) {
            for (auto& callback : event_callbacks_[(size_t)(event.Type())]) {
                callback(event);
            }
        }

    private:
        std::array<std::vector<EventCallback<Event>>, static_cast<size_t>(EventType::Count)> event_callbacks_;
    };

}  // namespace DummyEngine