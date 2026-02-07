#pragma once

#include "TimeSpan.hpp"
#include "System/String.hpp"

class TimePoint : public Object<TimePoint>
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

    constexpr Boolean operator<(TimePoint other) const noexcept
    {
        return _sinceEpoch < other._sinceEpoch;
    }

    String ToString() const noexcept
    {
        return _sinceEpoch.ToString();
    }
};