#pragma once

#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"

namespace DummyEngine {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)    = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>&               GetIndexBuffer() const   = 0;

        Ref<VertexArray>        Copy() const;
        static Ref<VertexArray> Create();
    };

}  // namespace DummyEngine