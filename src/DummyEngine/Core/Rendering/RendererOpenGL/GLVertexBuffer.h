#pragma once

#include <GLAD/glad.h>

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/VertexBuffer.h"

namespace DE
{

    class GLVertexBuffer : public VertexBuffer
    {
    public:
        GLVertexBuffer() = delete;
        GLVertexBuffer(const GLVertexBuffer& other) = delete;
        GLVertexBuffer(GLVertexBuffer&& other) = delete;
        GLVertexBuffer& operator=(const GLVertexBuffer& other) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer&& other) = delete;

        GLVertexBuffer(const BufferLayout& layout, uint32_t size, BufferUsage usage = BufferUsage::Static);
        GLVertexBuffer(const BufferLayout& layout, uint32_t size, const void* data, BufferUsage usage = BufferUsage::Static);

        virtual ~GLVertexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual LocalBufferNode at(uint32_t index) override;

        virtual void SetData(const void* data, uint32_t size) override;
        virtual void PushData() override;

        virtual const BufferLayout& GetLayout() const override;

    private:
        static GLenum BufferUsafeToGLBufferUsage(BufferUsage usage);

        BufferUsage m_Usage;
        LocalBuffer m_LocalBuffer;
        BufferLayout m_Layout;
        GLuint m_BufferId;
        uint32_t m_Size;
    };

    class GLIndexBuffer : public IndexBuffer
    {
    public:
        GLIndexBuffer() = delete;
        GLIndexBuffer(const GLIndexBuffer& other) = delete;
        GLIndexBuffer(GLIndexBuffer&& other) = delete;
        GLIndexBuffer& operator=(const GLIndexBuffer& other) = delete;
        GLIndexBuffer& operator=(GLIndexBuffer&& other) = delete;

        GLIndexBuffer(const uint32_t* indices, uint32_t count);
        virtual ~GLIndexBuffer();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual uint32_t IndicesAmount() const override;

    private:
        GLuint m_BufferId;
        uint32_t _indices_amount;
    };
}  // namespace DE