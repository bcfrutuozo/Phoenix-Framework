#pragma once

#include "System/Types.hpp"

class TimeSpan
{
public:
    using rep = i64; // nanoseconds

private:
    rep _ns;

public:
    constexpr TimeSpan() noexcept : _ns(0) {}
    explicit constexpr TimeSpan(rep nanoseconds) noexcept : _ns(nanoseconds) {}

    // ---------- factories ----------
    static constexpr TimeSpan FromNanoseconds(rep ns) noexcept { return TimeSpan(ns); }
    static constexpr TimeSpan FromMicroseconds(rep us) noexcept { return TimeSpan(us * 1'000); }
    static constexpr TimeSpan FromMilliseconds(rep ms) noexcept { return TimeSpan(ms * 1'000'000); }
    static constexpr TimeSpan FromSeconds(Double s) noexcept
    {
        return TimeSpan(static_cast<rep>(s * 1'000'000'000.0));
    }

    // ---------- access ----------
    constexpr rep Nanoseconds()  const noexcept { return _ns; }
    constexpr Double Seconds()   const noexcept { return Double(_ns) / 1'000'000'000.0; }

    // ---------- operators ----------
    constexpr TimeSpan operator+(TimeSpan other) const noexcept { return TimeSpan(_ns + other._ns); }
    constexpr TimeSpan operator-(TimeSpan other) const noexcept { return TimeSpan(_ns - other._ns); }

    constexpr Boolean operator<(TimeSpan other) const noexcept { return _ns < other._ns; }
    constexpr Boolean operator>(TimeSpan other) const noexcept { return _ns > other._ns; }
    constexpr Boolean operator==(TimeSpan other) const noexcept { return _ns == other._ns; }
};