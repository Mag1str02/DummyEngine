#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define DE_PLATFORM_WINDOWS
	#else
		#error "Win32 not supported."
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#error "Apple not supported."
#elif defined(__linux__)
	#define DE_PLATFORM_LINUX
#else
	#error "Unknown platform."
#endif 
