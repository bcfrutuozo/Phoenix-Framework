#pragma once

#include "System/Meta/Access.hpp"
#include "System/Meta/WrapperTraits.hpp"
#include "System/Meta/WrapperValue.hpp"
#include "System/Meta/CharTraits.hpp"
#include "System/Meta/PrimitiveToWrapper.hpp"
#include "System/Meta/PromotionPrimitives.hpp"
#include "System/Meta/Promotion.hpp"
#include "System/Meta/HierarchyTraits.hpp"
#include "System/Meta/TypeTraits.hpp"

#include "System/Types.hpp"

#include <stdint.h>

// ==========================================================
// Hash base (FNV-1a)
// ==========================================================

constexpr UInt32 FNV_OFFSET_32 = 2166136261u;
constexpr UInt32 FNV_PRIME_32 = 16777619u;

inline u32 HashBytes(const Pointer data, u64 size) noexcept
{
    const u8* bytes = (const u8*)data.Get();
    u32 hash = FNV_OFFSET_32;

    for (uint64_t i = 0; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= FNV_PRIME_32;
    }

    return hash;
}

template<typename T>
inline u32 Hash(const T& value) noexcept
{
    return HashBytes(&value, sizeof(T));
}

inline u32 Hash(u32 v) noexcept
{
    v ^= v >> 16;
    v *= 0x7feb352d;
    v ^= v >> 15;
    v *= 0x846ca68b;
    v ^= v >> 16;
    return v;
}

inline u32 Hash(i32 v) noexcept
{
    return Hash((u32)v);
}

inline u32 Hash(u64 v) noexcept
{
    v ^= v >> 33;
    v *= 0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= 0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
    return (u32)v;
}

template<typename T>
inline u32 Hash(T* ptr) noexcept
{
    return Hash((u64)(uintptr_t)ptr);
}

template<typename T>
inline u32 Hash(Pointer ptr) noexcept
{
    return Hash(static_cast<void*>(ptr.Get()));
}

template<typename E>
inline u32 HashEnum(E e) noexcept
{
    using U = underlying_type_t(E);
    return Hash((U)e);
}

template<typename T>
inline u32 Hash(const T& v) noexcept
{
    if constexpr (is_base_of_v<Object<T>, T>)
    {
        return v.GetHashCode();
    }
    else
    {
        return HashBytes(&v, sizeof(T));
    }
}