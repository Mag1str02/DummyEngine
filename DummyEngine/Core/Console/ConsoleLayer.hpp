#pragma once

#include "DummyEngine/Core/Application/Layer.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class ConsoleLayer : public Layer {
        LOG_AUTHOR(ConsoleLayer)
    public:
        void OnAttach() override;
        void OnUpdate(float dt) override;
        void OnImGuiRender() override;
        void OnDetach() override;

    private:
        bool        show_ = false;
        std::string command_;
        S32         history_position_ = 0;
        bool        scroll_to_bottom_ = true;
        bool        just_opened_      = false;
    };

}  // namespace DummyEngine