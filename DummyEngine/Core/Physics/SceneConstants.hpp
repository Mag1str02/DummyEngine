#pragma once

#include "DummyEngine/Utils/Types/Types.h"
namespace DE::Physics {

    class SceneConstants {

    public:
        const Vec3& getGravity() const;
        void        setGravity(const Vec3& gravity);

    private:
        Vec3 _gravity;
    };

}