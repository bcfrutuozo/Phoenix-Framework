#pragma once

#include "System/Types.hpp"

namespace Memory
{
    void Zero(Pointer p, u32 size) noexcept;

    void Copy(Pointer p, const Pointer src, u32 size) noexcept;

    Pointer Alloc(u64 size) noexcept;

    void Free(Pointer p) noexcept;

    static Pointer Realloc(Pointer p, u64 newSize) noexcept
    {
        Pointer n = Alloc(newSize);
        if (p)
        {
            // aqui você pode armazenar size no header futuramente
            Copy(n, p, newSize);
            Free(p);
        }
        return n;
    }

    void Move(Pointer dst, const Pointer src, u32 size) noexcept;

    i32 Compare(const Pointer a, const Pointer b, u32 size) noexcept;

    void Fill(Pointer dst, u8 value, u32 size) noexcept;

    inline constexpr Boolean IsAligned(const Pointer p, u32 alignment) noexcept
    {
        return ((u64)p & (alignment - 1)) == 0;
    }

    static Pointer AlignForward(Pointer p, u32 alignment) noexcept
    {
        return Pointer((static_cast<u64>(p) + (alignment - 1)) & ~(alignment - 1));
    }

    inline static constexpr u64 AlignForward(u64 value, u32 alignment) noexcept
    {
        return (value + (alignment - 1)) & ~(alignment - 1);
    }

    // ------------------------------------------------------------
    // Object lifetime
    // ------------------------------------------------------------

    template<typename T, typename... Args>
    static inline T* Construct(Pointer memory, Args&&... args) noexcept
    {
        return new (memory) T(static_cast<Args&&>(args)...);
    }

    template<typename T>
    static inline void Destroy(T* obj) noexcept
    {
        obj->~T();
    }

    // ------------------------------------------------------------
    // Page management
    // ------------------------------------------------------------

    enum class PageProtection : uint8_t
    {
        NoAccess,
        Read,
        Write,
        ReadWrite,
        Execute,
        ReadExecute,
        ReadWriteExecute
    };

    u64 PageSize() noexcept;

    Pointer AllocPages(u64 pageCount) noexcept;
    void  FreePages(Pointer p, u64 pageCount) noexcept;
    Boolean ProtectPages(Pointer p, u64 pageCount, PageProtection prot) noexcept;

    // ------------------------------------------------------------
    // Debug / safety
    // ------------------------------------------------------------
    inline static constexpr Boolean IsReadable(const Pointer p, u64 size) noexcept { return p != nullptr; }
    inline static constexpr Boolean IsWritable(Pointer p, u64 size) noexcept { return p != nullptr; }
};