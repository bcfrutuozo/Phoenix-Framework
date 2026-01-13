#pragma once

#include <cstdint>

namespace Phoenix
{
	extern uint64_t GLOBAL_HASH_SEED;

#ifdef _WIN32
	extern bool IsWin32NetworkInitialized;
#endif

	extern int32_t ScreenWidth;
	extern int32_t ScreenHeight;
}