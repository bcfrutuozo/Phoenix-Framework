#pragma once

#include "TimePoint.hpp"

class Clock
{
public:

    // Thread-safe
    static TimePoint Now() noexcept;

    // Pausa a thread atual (futuro: pode ser async)
    static void Sleep(TimeSpan duration) noexcept;
};