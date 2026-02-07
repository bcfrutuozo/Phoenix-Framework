#pragma once

#include "System/Types/Primitives/Boolean.hpp"
#include "System/Types/Fundamentals/Pointer.hpp"

struct SocketHandle
{

#ifdef _WIN32
    static constexpr Pointer Invalid = Pointer(static_cast<Pointer::value_type>(~0ull));
#else
    static constexpr Pointer Invalid = Pointer(static_cast<Pointer::value_type>(-1));
#endif

    Pointer Value = Invalid;

    inline constexpr Boolean IsValid() const noexcept { return Value != Invalid; }
    inline constexpr void Reset() noexcept { Value = Invalid; }
};