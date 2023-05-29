#pragma once

#ifdef DE_PLATFORM_WINDOWS
#include "DummyEngine/Platform/Windows/Utils/Debug/Demangler.h"
#endif
#ifdef DE_PLATFORM_LINUX
#include "DummyEngine/Platform/Linux/Utils/Debug/Demangler.h"
#endif
#ifdef DE_PLATFORM_MACOS
#include "DummyEngine/Platform/Macos/Utils/Debug/Demangler.h"
#endif