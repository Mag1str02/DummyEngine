#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() {}

        void SetActiveScene(Ref<Scene> scene);
        void OnImGuiRender(bool& enabled);
        
        Entity GetActiveEntity();

    private:
        Ref<Scene> m_Scene;
        Entity m_SelectedEntity;
    };
}  // namespace DE