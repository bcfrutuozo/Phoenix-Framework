#include "Globals.hpp"

namespace Phoenix
{
    uint64_t GLOBAL_HASH_SEED = 0;

#ifdef _WIN32
    bool IsWin32NetworkInitialized = false;
#endif

    int32_t ScreenWidth = 0;
    int32_t ScreenHeight = 0;
}