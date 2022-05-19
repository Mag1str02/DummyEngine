#pragma once
#include <GLAD/glad.h>

#include <unordered_map>

#include "../../../../Addition/types.h"

namespace DE {

class ElementBuffer {

public:
    ElementBuffer();
    ElementBuffer(const ElementBuffer& other);
    ElementBuffer(ElementBuffer&& other);
    ElementBuffer& operator=(const ElementBuffer& other);
    ElementBuffer& operator=(ElementBuffer&& other);
    ~ElementBuffer();

private:
    friend class Initializer;

    class ElementBufferManager {
    private:
        bool _initialized;
        std::unordered_map<BufferId, ReferenceCount> _reference_count;

        ElementBufferManager();

    public:
        static ElementBufferManager& Get();

        BufferId CreateElementBuffer();
        BufferId CreateInstance(BufferId buffer_id);
        void DestroyInstance(BufferId buffer_id);
        void DestroyElementBuffer(BufferId buffer_id);

        void Initialize();
        void Terminate();
    };

    BufferId _buffer_id;
};
}  // namespace DE