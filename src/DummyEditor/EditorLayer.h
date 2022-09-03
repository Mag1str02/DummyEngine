#pragma once

#include "DummyEngineInclude.h"
#include "Panels/ViewportPanel.h"

namespace DE
{
    struct SceneData
    {
        Ref<Scene> scene;
        Ref<FrameBuffer> frame_buffer;
    };

    class EditorLayer : public DE::Layer
    {
    public:
        EditorLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate(float dt) override;
        virtual void OnImGuiRender() override;

    private:
        void ShowDockingSpace();
        
        void OpenScene(const Path& path);

        ViewportPanel m_Viewport;
        SceneData m_SceneData;
    };
}  // namespace DE