#pragma once

#include <cstdint>

inline uint64_t GLOBAL_HASH_SEED;

#ifdef _WIN32
inline static bool IsWin32NetworkInitialized = false;
#endif