#pragma once

#include "DummyEngine/Core/ResourceManaging/Resources/ShaderPart.hpp"

namespace DE {

    class ShaderPart {
    public:
        virtual ~ShaderPart() = default;

        static Ref<ShaderPart> Create(const ShaderPartResource&data);

    };

}