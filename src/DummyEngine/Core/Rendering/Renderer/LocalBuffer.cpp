#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"

namespace DE
{
    //*~~~LocalBufferNode~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBufferNode::LocalBufferNode(LocalBuffer* buffer, uint8_t* pointer) : m_Buffer(buffer), m_Pointer(pointer) {}
    LocalBufferNode& LocalBufferNode::operator=(const LocalBufferNode& other)
    {
        DE_ASSERT(m_Buffer == other.m_Buffer, "Trying to equeal LocalBufferNodes from diffrent LocalBuffers.");
        std::memcpy(m_Pointer, other.m_Pointer, m_Buffer->m_Layout.GetStride());
        return *this;
    }

    //*~~~LocalBuffer~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBuffer::LocalBuffer() : m_Data(nullptr), m_Size(0) {}
    LocalBuffer::~LocalBuffer()
    {
        if (m_Data)
        {
            delete[] m_Data;
        }
    }
    LocalBufferNode LocalBuffer::at(uint32_t index) { return LocalBufferNode(this, m_Data + index * m_Layout.GetStride()); }
    void            LocalBuffer::SetData(const void* data, uint32_t size)
    {
        DE_ASSERT(m_Size == size, "Wrong data size!");
        memcpy(m_Data, data, m_Size);
    }
    void LocalBuffer::Allocate(const BufferLayout& layout, uint32_t size)
    {
        m_Layout = layout;
        Allocate(size * m_Layout.GetStride());
    }
    void LocalBuffer::Allocate(uint32_t size)
    {
        if (m_Data)
        {
            delete[] m_Data;
        }
        m_Data = (uint8_t*)malloc(size);
        m_Size = size;
    }

}  // namespace DE