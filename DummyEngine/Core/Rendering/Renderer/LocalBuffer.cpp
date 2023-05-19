#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"

namespace DE {
    //*~~~LocalBufferNode~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBufferNode::LocalBufferNode(LocalBuffer* buffer, U8* pointer) : m_Buffer(buffer), m_Pointer(pointer) {}
    LocalBufferNode& LocalBufferNode::operator=(const LocalBufferNode& other) {
        DE_ASSERT(m_Buffer == other.m_Buffer, "Trying to assign LocalBufferNodes from diffrent LocalBuffers");
        std::memcpy(m_Pointer, other.m_Pointer, m_Buffer->m_Layout.GetStride());
        return *this;
    }

    //*~~~LocalBuffer~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    LocalBuffer::LocalBuffer() : m_Data(nullptr), m_Size(0) {}
    LocalBuffer::~LocalBuffer() {
        if (m_Data) {
            delete[] m_Data;
        }
    }
    LocalBufferNode LocalBuffer::at(U32 index) {
        return LocalBufferNode(this, m_Data + index * m_Layout.GetStride());
    }
    void LocalBuffer::SetData(const void* data, U32 size) {
        DE_ASSERT(m_Size == size, "Wrong data size (", size, ") expected (", m_Size, ")");
        memcpy(m_Data, data, m_Size);
    }
    void LocalBuffer::Allocate(const BufferLayout& layout, U32 size) {
        m_Layout = layout;
        Allocate(size * m_Layout.GetStride());
    }
    void LocalBuffer::Allocate(U32 size) {
        if (m_Data) {
            delete[] m_Data;
        }
        m_Data = (U8*)malloc(size);
        m_Size = size;
    }

}  // namespace DE