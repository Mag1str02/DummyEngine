#include "DummyEngine/Core/Rendering/Renderer/VertexBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexBuffer.h"

namespace DE
{
    BufferElement::BufferElement(BufferElementType type, bool normalized) :
        type(type),
        normalized(normalized),
        size(SizeOfElementType(type))
    {}
    uint32_t BufferElement::SizeOfElementType(BufferElementType type)
    {
        switch (type)
        {
            case BufferElementType::Float: return 4;
            case BufferElementType::Float2: return 8;
            case BufferElementType::Float3: return 12;
            case BufferElementType::Float4: return 16;
            case BufferElementType::Int: return 4;
            case BufferElementType::Int2: return 8;
            case BufferElementType::Int3: return 12;
            case BufferElementType::Int4: return 16;
            default: return 0;
        }
    }
    uint32_t BufferElement::ComponentCount() const
    {
        switch (type)
        {
            case BufferElementType::Float: return 1;
            case BufferElementType::Float2: return 2;
            case BufferElementType::Float3: return 3;
            case BufferElementType::Float4: return 4;
            case BufferElementType::Int: return 1;
            case BufferElementType::Int2: return 2;
            case BufferElementType::Int3: return 3;
            case BufferElementType::Int4: return 4;
            default: return 0;
        }
    }

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }
    void BufferLayout::CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        for (auto& element : m_Elements)
        {
            element.offset = offset;
            offset += element.size;
        }
        m_Stride = offset;
    }

    std::vector<BufferElement>::iterator BufferLayout::begin()
    {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::iterator BufferLayout::end()
    {
        return m_Elements.end();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::begin() const
    {
        return m_Elements.begin();
    }
    std::vector<BufferElement>::const_iterator BufferLayout::end() const
    {
        return m_Elements.end();
    }

    uint32_t BufferLayout::GetStride() const
    {
        return m_Stride;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferUsage usage)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(size, usage);
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size, BufferUsage usage)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(data, size, usage);
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLIndexBuffer>(indices, count);
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }

}  // namespace DE