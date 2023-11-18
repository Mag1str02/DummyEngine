#pragma once

#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void Remove() = 0;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;

        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>&               GetIndexBuffer() const   = 0;

        Ref<VertexArray>        Copy() const;
        static Ref<VertexArray> Create();
    };
}  // namespace DE