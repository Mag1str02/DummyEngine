#pragma once

#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"
#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"

namespace DummyEngine {

    class UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;

        virtual void            Bind(U32 index) const = 0;
        virtual LocalBufferNode At(U32 index)         = 0;

        virtual void                PushData() const  = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static Ref<UniformBuffer> Create(const BufferLayout& layout, U32 size);
    };

}  // namespace DummyEngine
