#pragma once

#include "TimeSpan.hpp"

class TimePoint
{
    TimeSpan _sinceEpoch;

public:
    constexpr TimePoint() noexcept : _sinceEpoch() {}
    explicit constexpr TimePoint(TimeSpan sinceEpoch) noexcept
        : _sinceEpoch(sinceEpoch) {
    }

    constexpr TimeSpan SinceEpoch() const noexcept { return _sinceEpoch; }

    // ---------- arithmetic ----------
    constexpr TimePoint operator+(TimeSpan delta) const noexcept
    {
        return TimePoint(_sinceEpoch + delta);
    }

    constexpr TimeSpan operator-(TimePoint other) const noexcept
    {
        return _sinceEpoch - other._sinceEpoch;
    }

    constexpr bool operator<(TimePoint other) const noexcept
    {
        return _sinceEpoch < other._sinceEpoch;
    }
};