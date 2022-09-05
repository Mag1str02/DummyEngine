#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() {}

        void SetActiveScene(Ref<Scene> scene);
        void OnImGuiRender();

    private:
        Ref<Scene> m_Scene;
    };
}  // namespace DE