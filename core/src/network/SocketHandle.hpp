#pragma once

#include "meta/BaseDef.hpp"
#include "types/Pointer.hpp"
#include "types/Boolean.hpp"

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