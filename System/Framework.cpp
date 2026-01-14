#include "Framework.hpp"

#include <eh.h>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "NetworkRuntime.hpp"
#else
#include <time.h>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#endif
#endif

#include <exception>

// ---------------------------------------------------------
// Random seed generation
// ---------------------------------------------------------

uint64_t GenerateRandomSeed() noexcept
{
    uint64_t v = 0;

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    v = __rdtsc();
#elif defined(__i386__) || defined(__x86_64__)
    unsigned int lo = 0, hi = 0;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    v = ((uint64_t)hi << 32) | lo;
#endif

    if (v == 0)
    {
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER cnt;
        if (QueryPerformanceCounter(&cnt))
        {
            v = (uint64_t)cnt.QuadPart;
        }
        else
        {
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            v = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
        }
#else
#if defined(CLOCK_REALTIME)
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        {
            v = (uint64_t)ts.tv_sec ^ ((uint64_t)ts.tv_nsec << 32);
        }
        else
#endif
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            v = ((uint64_t)tv.tv_sec << 32) ^ (uint64_t)tv.tv_usec;
        }
#endif
    }

    uint64_t mix = (uint64_t)(uintptr_t)&GenerateRandomSeed;
    v ^= mix + 0x9E3779B97F4A7C15ULL + (v << 6) + (v >> 2);

    if (v == 0)
        v = 0xA5A5A5A5A5A5A5A5ULL;

    return v;
}

// ---------------------------------------------------------
// Framework initialization
// ---------------------------------------------------------

void InitFramework()
{
    EnableDivideByZeroTrapGlobal(1);

    Phoenix::GLOBAL_HASH_SEED = GenerateRandomSeed();

#if defined(_WIN32) || defined(_WIN64)
    NetworkRuntime::EnsureInitialized();

    Phoenix::ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    Phoenix::ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
#endif
}

void CloseFramework()
{
#if defined(_WIN32) || defined(_WIN64)
    NetworkRuntime::CloseNetwork();
#endif
}