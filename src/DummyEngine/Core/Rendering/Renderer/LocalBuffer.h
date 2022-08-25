#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/BufferLayout.h"

namespace DE
{
    class LocalBufferNode;
    class LocalBuffer
    {
    public:
        LocalBuffer();
        ~LocalBuffer();

        void SetData(const void* data, uint32_t size);
        void Allocate(const BufferLayout& layout, uint32_t size);

        LocalBufferNode at(uint32_t index);

    private:
        void Allocate(uint32_t size);

        friend class GLVertexBuffer;
        friend class LocalBufferNode;

        BufferLayout m_Layout;
        uint8_t* m_Data;
        uint32_t m_Size;
    };
    class LocalBufferNode
    {
    public:
        template <typename T> T& Get(uint32_t index)
        {
            return *(T*)(m_Pointer + m_Buffer->m_Layout[index].offset);
        }
        LocalBufferNode& operator=(const LocalBufferNode& other);

    private:
        LocalBufferNode(LocalBuffer* buffer, uint8_t* pointer);

        friend class LocalBuffer;

        LocalBuffer* m_Buffer;
        uint8_t* m_Pointer;
    };
}  // namespace DE