#include "DummyEngine/Platform/PlatformResolver.h"

#ifdef DE_PLATFORM_WINDOWS
#include "DummyEngine/Platform/Windows/Utils/FileDialogs.cpp"
#endif

#ifdef DE_PLATFORM_LINUX
#include "DummyEngine/Platform/Linux/Utils/FileDialogs.cpp"
#endif