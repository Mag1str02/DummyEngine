#pragma once
#include <GLAD/glad.h>

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"

namespace DE
{
    class GLVertexArray : public VertexArray
    {
    public:
        GLVertexArray();
        virtual ~GLVertexArray();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;

        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override;

    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;

        GLuint m_ArrayId;
        uint32_t m_CurrentAtributeId;
    };
}  // namespace DE