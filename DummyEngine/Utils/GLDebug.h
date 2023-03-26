#pragma once

namespace DE
{
    void glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)
}  // namespace DE
