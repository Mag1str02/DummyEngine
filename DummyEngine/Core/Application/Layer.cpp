#include "Layer.h"

#include "Application.h"

namespace DummyEngine {

    Layer::Layer(const std::string& name) : m_Name(name) {}
    const std::string& Layer::GetName() const {
        return m_Name;
    }
    void Layer::BroadcastEvent(const Event& event) {
        Application::OnEvent(event);
    }

}  // namespace DummyEngine