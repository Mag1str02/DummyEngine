#pragma once

#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"

#include <glad/glad.h>

namespace DummyEngine {

    class GLVertexArray : public VertexArray {
    public:
        GLVertexArray();
        virtual ~GLVertexArray();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;

        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
        virtual const Ref<IndexBuffer>&               GetIndexBuffer() const override;

    private:
        std::vector<Ref<VertexBuffer>> vertex_buffers_;
        Ref<IndexBuffer>               index_buffer_;

        GLuint array_id_;
        U32    current_attribute_id_;
    };

}  // namespace DummyEngine