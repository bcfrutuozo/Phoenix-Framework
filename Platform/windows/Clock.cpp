#include "System/Time/Clock.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static inline Int64 GetQPCFrequency() noexcept
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return static_cast<Int64>(freq.QuadPart);
}

TimePoint Clock::Now() noexcept
{
    static const Int64 frequency = GetQPCFrequency();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    // Convert ticks → nanoseconds
    Int64 ns =
        (counter.QuadPart * 1'000'000'000LL) / frequency;

    return TimePoint(TimeSpan::FromNanoseconds(ns));
}

void Clock::Sleep(TimeSpan duration) noexcept
{
    if (duration.Nanoseconds() <= 0)
        return;

    // Sleep() works in milliseconds (rounded up)
    DWORD ms = static_cast<DWORD>((uint64_t)(duration.Nanoseconds() + 999'999) / 1'000'000);

    ::Sleep(ms);
}