#pragma once

// ============================================
//  DSCPP Framework Global Initializer
//  - Executa automaticamente antes da main()
//  - Inicializa traps, handlers, registries, etc.
//  - Não usa std: completamente freestanding-friendly
// ============================================

#include "Globals.hpp"
#include "DivideByZeroTrap.hpp"
#include "Exceptions.hpp"
#include "Types.hpp"
#include "interop/Console.hpp"
#include "platform/ApplicationFactory.hpp"

#include <stdint.h>
#include <stddef.h>

#if defined(_MSC_VER)
#include <intrin.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <profileapi.h>
#include "NetworkRuntime.hpp"
#endif
#else
#include <time.h>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#endif
#endif

// Prototipo do seu init real
static void InitFramework();

// ---------------------------------------------------------
// Caso 1: Usar atributo constructor (Linux, macOS, GCC/Clang)
// ---------------------------------------------------------
#if defined(__GNUC__) || defined(__clang__)
__attribute__((constructor))
static void __dscpp_global_init_constructor() {
    InitFramework();
}
#endif

// ---------------------------------------------------------
// Caso 2: MSVC (Windows) — usar init_seg + global object
// ---------------------------------------------------------
#if defined(_MSC_VER)

#pragma init_seg(".CRT$XCU")  
// Isso garante execução após CRT básico, porém antes da main

struct __dscpp_GlobalInitializerWin {
    __dscpp_GlobalInitializerWin() {
        InitFramework();
    }
};

// Instância global → executado antes da main
static __dscpp_GlobalInitializerWin __dscpp_initializer_instance;

#endif

static inline uint64_t GenerateRandomSeed() noexcept
{
    uint64_t v = 0;

    // 1) try RDTSC / __rdtsc
#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_IX86))
    v = __rdtsc();
#elif defined(__i386__) || defined(__x86_64__)
    unsigned int lo = 0, hi = 0;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    v = ((uint64_t)hi << 32) | lo;
#endif

    // 2) If rdtsc not available or returned 0, use high-res timers
    if (v == 0) {
#if defined(_WIN32) || defined(_WIN64)
        LARGE_INTEGER cnt;
        if (QueryPerformanceCounter(&cnt)) {
            v = (uint64_t)cnt.QuadPart;
        }
        else {
            FILETIME ft;
            GetSystemTimeAsFileTime(&ft);
            v = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
        }
#else
#if defined(CLOCK_REALTIME)
        struct timespec ts;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
            v = (uint64_t)ts.tv_sec ^ ((uint64_t)ts.tv_nsec << 32);
        }
        else
#endif
        {
            struct timeval tv;
            gettimeofday(&tv, nullptr);
            v = ((uint64_t)tv.tv_sec << 32) ^ (uint64_t)tv.tv_usec;
        }
#endif
    }

    // 3) mix with address of this function (ASLR + variability) and some constant
    uint64_t mix = (uint64_t)(uintptr_t)&GenerateRandomSeed;
    v ^= mix + 0x9E3779B97F4A7C15ULL + (v << 6) + (v >> 2);

    // ensure non-zero
    if (v == 0) v = 0xA5A5A5A5A5A5A5A5ULL;

    return v;
}

// ==========================================================
//  Implementação padrão do InitFramework()
//  (Você pode mover isso para um .cpp se quiser)
// ==========================================================

static void InitFramework()
{
    // 1. Ativar traps globais (integer + float)
    //    = divide-by-zero vira exceção
    EnableDivideByZeroTrapGlobal(1 /*enable float traps*/);
    GLOBAL_HASH_SEED = GenerateRandomSeed();

#ifdef _WIN32
    NetworkRuntime::EnsureInitialized();
#endif

    // 2. (Opcional) Iniciar qualquer outro módulo no futuro:
    //    - Registradores unicode
    //    - Log
    //    - Pool de memória
    //    - Ajustes de alocador próprio
    //    - Handlers para InvalidOperation, etc.

    // Exemplos de você adicionar depois:
    //
    // UnicodeCase::Init();
    // DSCPP_Memory::Init();
    // DSCPP_Log::Init();
}