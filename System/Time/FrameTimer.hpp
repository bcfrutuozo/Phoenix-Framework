#pragma once

#include "TimeSpan.hpp"
#include "TimePoint.hpp"
#include "Clock.hpp"

class FrameTimer
{
    TimePoint _last;
    TimeSpan  _delta;

public:

    constexpr FrameTimer() noexcept
        : _last(Clock::Now()), _delta() {
    }

    constexpr void Tick() noexcept
    {
        TimePoint now = Clock::Now();
        _delta = now - _last;
        _last = now;
    }

    TimeSpan Delta() const noexcept { return _delta; }
    Double DeltaSeconds() const noexcept { return _delta.Seconds(); }
};