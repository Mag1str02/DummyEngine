#include "DummyEngine/Platform/PlatformResolver.h"

#ifdef DE_PLATFORM_WINDOWS
#include "DummyEngine/Platform/Windows/Utils/FileSystem.cpp"
#endif

#ifdef DE_PLATFORM_LINUX
#include "DummyEngine/Platform/Linux/Utils/FileSystem.cpp"
#endif