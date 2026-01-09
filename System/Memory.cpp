#include "Memory.hpp"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/mman.h>
#include <unistd.h>
#endif

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace Memory 
{
    Pointer Alloc(u64 size) noexcept
    {
#if defined(_WIN32)
        return Pointer(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
#else
        Pointer p = Pointer(mmap(nullPointer, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        return (p.Get() == MAP_FAILED) ? nullPointer : p;
#endif
    }

    void Free(Pointer p) noexcept
    {
#if defined(_WIN32)
        VirtualFree(p.Get(), 0, MEM_RELEASE);
#else
        // size não é necessário para munmap se você guardar em header interno
        munmap(p, 0); // você vai substituir isso quando criar seu allocator
#endif
    }

    inline void Zero(Pointer p, u32 size) noexcept
    {
#if defined(__clang__)
        __builtin_memset_inline(Pointer, 0, size);
#elif defined(__GNUC__)
        __builtin_memset(Pointer, 0, size);
#elif defined(_MSC_VER)
        __stosb(reinterpret_cast<unsigned char*>(p.Get()), 0, size);
#else
        unsigned char* p = static_cast<unsigned char*>(p);
        for (u32 i = 0; i < size; ++i)
            p[i] = 0;
#endif
    }

    // ------------------------------------------------------------
    // Copy memory
    // ------------------------------------------------------------
    void Copy(Pointer dst, const Pointer src, u32 size) noexcept
    {
#if defined(__clang__) || defined(__GNUC__)
        __builtin_memcpy(dst, src, size);
#elif defined(_MSC_VER)
        __movsb(
            reinterpret_cast<unsigned char*>(dst.Get()),
            reinterpret_cast<const unsigned char*>(src.Get()),
            size
        );
#else
        unsigned char* d = static_cast<unsigned char*>(dst);
        const unsigned char* s = static_cast<const unsigned char*>(src);
        for (uint32_t i = 0; i < size; ++i)
            d[i] = s[i];
#endif
    }

    void Move(Pointer dst, const Pointer src, u32 size) noexcept
    {
#if defined(__clang__) || defined(__GNUC__)
        __builtin_memmove(dst, src, size);
#else
        Copy(dst, src, size);
#endif
    }

    i32 Compare(const Pointer a, const Pointer b, u32 size) noexcept
    {
#if defined(__clang__) || defined(__GNUC__)
        return __builtin_memcmp(a, b, size);
#else
        const unsigned char* x = (const unsigned char*)a.Get();
        const unsigned char* y = (const unsigned char*)b.Get();
        for (uint32_t i = 0; i < size; ++i)
            if (x[i] != y[i]) return (i32)x[i] - (i32)y[i];
        return 0;
#endif
    }

    void Fill(Pointer dst, u8 value, u32 size) noexcept
    {
#if defined(__clang__) || defined(__GNUC__)
        __builtin_memset(dst, value, size);
#else
        unsigned char* p = (unsigned char*)dst.Get();
        for (uint32_t i = 0; i < size; ++i) p[i] = value;
#endif
    }

    u64 PageSize() noexcept
    {
#if defined(_WIN32)
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return (uint64_t)si.dwPageSize;
#else
        return (uint64_t)sysconf(_SC_PAGESIZE);
#endif
    }

    Pointer AllocPages(u64 pageCount) noexcept
    {
        const uint64_t size = pageCount * PageSize();

#if defined(_WIN32)
        return Pointer(VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
#else
        void* p = mmap(nullPointer, size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1, 0);

        return (p == MAP_FAILED) ? Pointer(nullPointer) : Pointer(p);
#endif
    }

    void FreePages(Pointer p, u64 pageCount) noexcept
    {
        if (!p) return;

        const uint64_t size = pageCount * PageSize();

#if defined(_WIN32)
        VirtualFree(p.Get(), 0, MEM_RELEASE);
#else
        munmap(p.Get(), size);
#endif
    }

    Boolean ProtectPages(Pointer p, u64 pageCount, PageProtection prot) noexcept
    {
        if (!p) return false;

        const u64 size = pageCount * PageSize();

#if defined(_WIN32)

        DWORD winProt = 0;
        switch (prot)
        {
        case PageProtection::NoAccess:          winProt = PAGE_NOACCESS; break;
        case PageProtection::Read:              winProt = PAGE_READONLY; break;
        case PageProtection::Write:             winProt = PAGE_READWRITE; break;
        case PageProtection::ReadWrite:         winProt = PAGE_READWRITE; break;
        case PageProtection::Execute:           winProt = PAGE_EXECUTE; break;
        case PageProtection::ReadExecute:       winProt = PAGE_EXECUTE_READ; break;
        case PageProtection::ReadWriteExecute:  winProt = PAGE_EXECUTE_READWRITE; break;
        default: return false;
        }

        DWORD oldProt;
        return VirtualProtect(p.Get(), size, winProt, &oldProt) != 0;

#else

        i32 unixProt = 0;
        switch (prot)
        {
        case PageProtection::NoAccess:          unixProt = PROT_NONE; break;
        case PageProtection::Read:              unixProt = PROT_READ; break;
        case PageProtection::Write:             unixProt = PROT_WRITE; break;
        case PageProtection::ReadWrite:         unixProt = PROT_READ | PROT_WRITE; break;
        case PageProtection::Execute:           unixProt = PROT_EXEC; break;
        case PageProtection::ReadExecute:       unixProt = PROT_READ | PROT_EXEC; break;
        case PageProtection::ReadWriteExecute:  unixProt = PROT_READ | PROT_WRITE | PROT_EXEC; break;
        default: return false;
        }

        return mprotect(p.Get(), size, unixProt) == 0;

#endif
    }
}