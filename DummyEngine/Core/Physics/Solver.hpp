#pragma once

#include "DummyEngine/Core/Scene/Scene.h"
#include "SceneConstants.hpp"
namespace DE::Physics {

    class Solver {
    public:
        void LoadScene(const Ref<Scene> &scene);

        void OnUpdate(double dt);
        double NextInteraction(double dt);

    private:
        Ref<Scene> _scene;
        Ref<SceneConstants> _constants;
        double _time = 0;
    };

}