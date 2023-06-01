#pragma once

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class GLVertexBuffer : public VertexBuffer {
    public:
        GLVertexBuffer()                                       = delete;
        GLVertexBuffer(const GLVertexBuffer& other)            = delete;
        GLVertexBuffer(GLVertexBuffer&& other)                 = delete;
        GLVertexBuffer& operator=(const GLVertexBuffer& other) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer&& other)      = delete;

        GLVertexBuffer(const BufferLayout& layout, U32 size, BufferUsage usage = BufferUsage::Static);
        GLVertexBuffer(const BufferLayout& layout, U32 size, const void* data, BufferUsage usage = BufferUsage::Static);

        virtual ~GLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual LocalBufferNode at(U32 index) override;

        virtual void SetData(const void* data, U32 size) override;
        virtual void PushData() override;

        virtual const BufferLayout& GetLayout() const override;
        virtual U32                 Size() const override;

    private:
        static GLenum BufferUsafeToGLBufferUsage(BufferUsage usage);

        BufferUsage  m_Usage;
        LocalBuffer  m_LocalBuffer;
        BufferLayout m_Layout;
        GLuint       m_BufferId;
        U32          m_Size;
    };

    class GLIndexBuffer : public IndexBuffer {
    public:
        GLIndexBuffer()                                      = delete;
        GLIndexBuffer(const GLIndexBuffer& other)            = delete;
        GLIndexBuffer(GLIndexBuffer&& other)                 = delete;
        GLIndexBuffer& operator=(const GLIndexBuffer& other) = delete;
        GLIndexBuffer& operator=(GLIndexBuffer&& other)      = delete;

        GLIndexBuffer(const U32* indices, U32 count);
        virtual ~GLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual U32 IndicesAmount() const override;

    private:
        GLuint m_BufferId;
        U32    _indices_amount;
    };
}  // namespace DE