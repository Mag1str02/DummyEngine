#pragma once

#ifdef DE_PLATFORM_WINDOWS
#include "DummyEngine/Platform/Windows/Utils/Debug/Demangler.h"
#endif

#ifdef DE_PLATFORM_APPLE
#include "DummyEngine/Platform/Macos/Utils/Debug/Demangler.h"
#endif