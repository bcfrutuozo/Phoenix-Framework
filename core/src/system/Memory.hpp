#pragma once

#include "meta/BaseDef.hpp"
#include "types/Byte.hpp"

struct Memory
{
    // ------------------------------------------------------------
    // Zero memory
    // ------------------------------------------------------------
    static inline void Zero(void* ptr, uint32_t size) noexcept
    {
        Byte* p = static_cast<Byte*>(ptr);
        for (uint32_t i = 0; i < size; ++i)
            p[i] = 0;
    }

    // ------------------------------------------------------------
    // Copy memory
    // ------------------------------------------------------------
    static inline void Copy(void* dst, const void* src, uint32_t size) noexcept
    {
        Byte* d = static_cast<Byte*>(dst);
        const Byte* s = static_cast<const Byte*>(src);

        for (uint32_t i = 0; i < size; ++i)
            d[i] = s[i];
    }
};