#pragma once

#include <glm/glm.hpp>
#include "../../../Wrappings/Window/window.h"

namespace DE{
    class GLRenderer{
        public:
            static bool Load(const Window* window);
            static void SetViewport(uint32_t width, uint32_t height, uint32_t x_offset, uint32_t y_offset);

            static void SetDefaultState();

            static void Clear();
            static void SetClearColor(glm::vec4 color);
            static void SetClearColor(float r, float g, float b, float a);
            
        private:
    };
}