#pragma once

namespace DummyEngine {
    void GlCheckError(const char* file, int line);
#define glCheckError() GlCheckError(__FILE__, __LINE__)
}  // namespace DummyEngine
