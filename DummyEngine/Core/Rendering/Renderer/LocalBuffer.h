#pragma once

#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    // TODO: Weak ptr stuff
    class LocalBufferNode;
    class LocalBuffer {
    public:
        LocalBuffer();
        ~LocalBuffer();

        void SetData(const void* data, U32 size);
        void Allocate(const BufferLayout& layout, U32 size);

        LocalBufferNode at(U32 index);

    private:
        void Allocate(U32 size);

        friend class GLVertexBuffer;
        friend class GLUniformBuffer;
        friend class LocalBufferNode;

        BufferLayout m_Layout;
        U8*     m_Data;
        U32     m_Size;
    };
    class LocalBufferNode {
    public:
        template <typename T> T& Get(U32 index) { return *(T*)(m_Pointer + m_Buffer->m_Layout[index].offset); }
        LocalBufferNode&         operator=(const LocalBufferNode& other);

    private:
        LocalBufferNode(LocalBuffer* buffer, U8* pointer);

        friend class LocalBuffer;

        LocalBuffer* m_Buffer;
        U8*     m_Pointer;
    };
}  // namespace DE