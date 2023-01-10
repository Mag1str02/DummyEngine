#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"
#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"

namespace DE
{

    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;

        virtual void            Bind(uint32_t index) const = 0;
        virtual LocalBufferNode at(uint32_t index)         = 0;

        virtual void                PushData() const  = 0;
        virtual const BufferLayout& GetLayout() const = 0;

        static Ref<UniformBuffer> Create(const BufferLayout& layout, uint32_t size);
    };
}  // namespace DE
