#include "DummyEngine/Platform/PlatformResolver.h"

#ifdef DE_PLATFORM_WINDOWS
#include "DummyEngine/Platform/Windows/Utils/FileSystem.cpp"
#include "DummyEngine/Platform/Windows/Core/Scripting/Compiler.cpp"
#include "DummyEngine/Platform/Windows/Core/Scripting/SharedObject.cpp"
#endif

#ifdef DE_PLATFORM_LINUX
#include "DummyEngine/Platform/Linux/Utils/FileSystem.cpp"
#include "DummyEngine/Platform/Linux/Core/Scripting/Compiler.cpp"
#include "DummyEngine/Platform/Linux/Core/Scripting/SharedObject.cpp"
#endif