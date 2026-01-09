#pragma once

// DivideByZeroTrap.hpp
// Cross-platform helper to trap CPU division-by-zero and map to a C++ exception.
// - Windows: installs SEH -> C++ translator and (optionally) enables FP traps.
// - POSIX : installs SIGFPE handler and (optionally) enables FP traps via feenableexcept.
// Usage patterns:
//  - EnableDivideByZeroTrapGlobal(true /*enableFloatTraps*/);  // installs global handlers
//  - RunWithDivideByZeroGuard([](){ /* code that might divide by zero */ }); // safe: throws DivideByZeroException
//
// IMPORTANT:
//  - Throwing from a signal handler is unsafe on POSIX. We therefore use sigsetjmp/siglongjmp in guarded blocks.
//  - Global behavior can catch many cases but guarded blocks are the only fully portable safe way to convert traps -> exceptions.

#include <stddef.h>
#include <signal.h>

extern "C" {
#include <setjmp.h>
}

#if defined(_WIN32) || defined(_WIN64)
// Windows NÃO tem sigjmp_buf
typedef jmp_buf dsc_sigjmp_buf;

#define dsc_sigsetjmp(env, save_mask) setjmp(env)
#define dsc_siglongjmp(env, val) longjmp(env, val)

#else
// Linux / macOS → usa sigjmp_buf corretamente
typedef sigjmp_buf dsc_sigjmp_buf;

#define dsc_sigsetjmp(env, save_mask) sigsetjmp(env, save_mask)
#define dsc_siglongjmp(env, val) siglongjmp(env, val)

#endif

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#  include <float.h>    // _controlfp_s
#  include <windows.h>
#  include <eh.h>
#else
#  include <fenv.h>     // feenableexcept
#  include <string.h>   // memset
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/ucontext.h>
#  include <errno.h>
#  include <stdlib.h>
#  include <stdio.h>
#  include <signal.h>
#  include <setjmp.h>
#endif

#include "Exceptions.hpp" // contains DivideByZeroException and BaseException

// ---------------------------
// Thread-local jmp buffer pointer for guarded blocks
// ---------------------------
#if defined(__STDC_NO_THREADS__) || (!defined(__cplusplus) && !defined(_MSC_VER))
    // fallback to pthreads? but keep simple: use compiler TLS where available
#endif

// Use C11/C++11 thread-local storage if available
#if defined(_MSC_VER)
#define DSC_THREAD_LOCAL __declspec(thread)
#else
#define DSC_THREAD_LOCAL __thread
#endif

DSC_THREAD_LOCAL dsc_sigjmp_buf* dsc_current_jmpbuf = nullptr;

// Save previous handlers / states for restore (global)
static struct {
#if defined(_WIN32) || defined(_WIN64)
    _se_translator_function prev_translator;
    unsigned int prev_control;
#else
    struct sigaction prev_sigfpe_action;
    int fe_exceptions_enabled; // 0/1, whether we enabled feenableexcept
#endif
    int installed;
} dsc_trap_state = { 0 };

// ---------------------------
// POSIX: SIGFPE handler (calls siglongjmp if guarded, otherwise calls default/abort)
// ---------------------------
#if !defined(_WIN32) && !defined(_WIN64)
static void dsc_sigfpe_handler(int signo, siginfo_t* info, void* context) {
    (void)context;
    // signo should be SIGFPE
    if (signo != SIGFPE) {
        // not ours: chain to previous if any
        if (dsc_trap_state.prev_sigfpe_action.sa_sigaction) {
            dsc_trap_state.prev_sigfpe_action.sa_sigaction(signo, info, context);
            return;
        }
        // fallback: restore default and re-raise
        signal(signo, SIG_DFL);
        raise(signo);
        return;
    }

    // If there is a guarded jmpbuf, longjmp back with code 1
    if (dsc_current_jmpbuf) {
        // siglongjmp to guarded point
        siglongjmp(*dsc_current_jmpbuf, 1);
        // unreachable
    }

    // No guarded context: try chaining to previous handler if it exists
    if (dsc_trap_state.prev_sigfpe_action.sa_sigaction) {
        dsc_trap_state.prev_sigfpe_action.sa_sigaction(signo, info, context);
        return;
    }

    // Otherwise abort (we cannot throw safely here)
    _exit(128 + SIGFPE);
}
#endif

// ---------------------------
// Windows: SE translator function (translates EXCEPTION_INT_DIVIDE_BY_ZERO, EXCEPTION_FLT_DIVIDE_BY_ZERO to C++ exceptions)
// ---------------------------
#if defined(_WIN32) || defined(_WIN64)
static void __cdecl dsc_se_translator(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
    (void)ep;
    // Map integer divide-by-zero and float divide-by-zero
    if (code == EXCEPTION_INT_DIVIDE_BY_ZERO || code == EXCEPTION_INT_OVERFLOW) {
        throw DivideByZeroException();
    }
    // For float divides, depending on control word, EXCEPTION_FLT_DIVIDE_BY_ZERO may be raised
    if (code == EXCEPTION_FLT_DIVIDE_BY_ZERO) {
        throw DivideByZeroException();
    }
    // otherwise rethrow as generic invalid operation
    throw InvalidOperationException("Unhandled structured exception code");
}
#endif

// ---------------------------
// Enable float traps (platform-specific)
// ---------------------------
static void dsc_enable_floating_point_traps_platform() {
#if defined(_WIN32) || defined(_WIN64)
    // Enable floating point exceptions: unmask divide-by-zero
    unsigned int current;
    _controlfp_s(&current, 0, 0); // get current
    unsigned int newMask = current & ~(_EM_ZERODIVIDE); // clear mask for zero divide => enable trap
    unsigned int ignored;
    _controlfp_s(&ignored, newMask, _MCW_EM);
#else
    // POSIX: feenableexcept if available
#if defined(__GNU__) || defined(__linux__) || defined(__APPLE__)
    // Try enabling FE_DIVBYZERO; if feenableexcept unavailable, ignore
#ifdef FE_DIVBYZERO
    // feenableexcept may not be declared as non-static in some platforms; call if available
    // We ignore errors: if feenableexcept fails, user still can use guarded blocks
    feenableexcept(FE_DIVBYZERO);
    dsc_trap_state.fe_exceptions_enabled = 1;
#endif
#endif
#endif
}

// ---------------------------
// Disable float traps (platform-specific)
// ---------------------------
static void dsc_disable_floating_point_traps_platform() {
#if defined(_WIN32) || defined(_WIN64)
    unsigned int ignored;
    // Mask divide-by-zero exception again (disable trap)
    _controlfp_s(&ignored, _EM_ZERODIVIDE, _MCW_EM);
#else
#ifdef FE_DIVBYZERO
    // No portable standard to disable; but on glibc you can use fedisableexcept.
#if defined(__GLIBC__)
    fedisableexcept(FE_DIVBYZERO);
#endif
    dsc_trap_state.fe_exceptions_enabled = 0;
#endif
#endif
}

// ---------------------------
// Public: Enable global trap/handler
// - enableFloatTraps: if true, try to enable IEEE float divide-by-zero traps (platform dependent).
// Returns 0 on success, non-zero on partial failure.
// ---------------------------
static int EnableDivideByZeroTrapGlobal(int enableFloatTraps) {
    if (dsc_trap_state.installed) return 0; // already installed

#if defined(_WIN32) || defined(_WIN64)
    // Save previous translator (can't fetch previous easily; keep null)
    dsc_trap_state.prev_translator = nullptr;
    // Install translator
    _set_se_translator(dsc_se_translator);
    if (enableFloatTraps) dsc_enable_floating_point_traps_platform();
    dsc_trap_state.installed = 1;
    return 0;
#else
    // POSIX: install SIGFPE handler
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = dsc_sigfpe_handler;
    act.sa_flags = SA_SIGINFO | SA_NODEFER;

    if (sigaction(SIGFPE, &act, &dsc_trap_state.prev_sigfpe_action) != 0) {
        // failed to install handler
        dsc_trap_state.installed = 0;
        return errno ? errno : -1;
    }

    if (enableFloatTraps) dsc_enable_floating_point_traps_platform();

    dsc_trap_state.installed = 1;
    return 0;
#endif
}

// ---------------------------
// Public: Disable global trap/handler (restores previous behavior)
// ---------------------------
static void DisableDivideByZeroTrapGlobal() {
    if (!dsc_trap_state.installed) return;

#if defined(_WIN32) || defined(_WIN64)
    // No direct way to restore previous translator (MSVC doesn't provide retrieval), so set to null translator
    _set_se_translator(nullptr);
    dsc_disable_floating_point_traps_platform();
    dsc_trap_state.installed = 0;
#else
    // restore previous handler
    sigaction(SIGFPE, &dsc_trap_state.prev_sigfpe_action, nullptr);
    if (dsc_trap_state.fe_exceptions_enabled) dsc_disable_floating_point_traps_platform();
    dsc_trap_state.installed = 0;
#endif
}

// ---------------------------
// Guarded execution: run callable inside sigsetjmp region. If a SIGFPE occurs, we convert to C++ exception.
// Usage:
//    RunWithDivideByZeroGuard([](){ /* code */ });
// or with function pointer:
//    RunWithDivideByZeroGuard(funcPtr, ctx);
// Returns 0 on success, non-zero if trapped (but function will throw DivideByZeroException).
// It will rethrow DivideByZeroException on trap.
// ---------------------------
template<typename Callable>
static void RunWithDivideByZeroGuard(Callable&& fn) {
    // Setup local jmpbuf on stack
    dsc_sigjmp_buf local_buf;

    // point thread-local pointer to our buffer
    dsc_current_jmpbuf = &local_buf;

    // Portable setjmp (Windows: setjmp, POSIX: sigsetjmp)
    if (dsc_sigsetjmp(local_buf, 1) == 0) {
        try {
            fn();
        }
        catch (...) {
            dsc_current_jmpbuf = nullptr;
            throw;
        }
        dsc_current_jmpbuf = nullptr;
        return;
    }
    else {
        dsc_current_jmpbuf = nullptr;
        throw DivideByZeroException();
    }
}


// ---------------------------
// Convenience overload for function pointer with void* context
// ---------------------------
typedef void(*dsc_guard_fn_t)(void*);
static void RunWithDivideByZeroGuard_fn(dsc_guard_fn_t f, void* ctx) {
    RunWithDivideByZeroGuard([=]() {
        f(ctx);
        });
}

// ---------------------------
// Example helper: evaluate an expression safely and return whether it threw DivideByZeroException.
// This is just syntactic sugar.
// ---------------------------
template<typename Callable>
static int TryRunCatchDivideByZero(Callable&& fn) {
    try {
        RunWithDivideByZeroGuard(fn);
        return 0;
    }
    catch (const DivideByZeroException&) {
        return 1;
    }
}

// ---------------------------
// Notes for usage (summary):
//  1) Prefer RunWithDivideByZeroGuard for code that MUST convert traps to exceptions portably.
//     Example:
//         RunWithDivideByZeroGuard([](){ int x = 1/0; });
//     This will throw DivideByZeroException on POSIX and Windows.
//  2) If you want global behavior, call EnableDivideByZeroTrapGlobal(true) early in main.
//     - On Windows this will translate SEH to C++ exceptions automatically.
//     - On POSIX this installs a SIGFPE handler and (optionally) enables FP traps; however,
//       throwing from signal handler is unsafe; the global handler will abort if a SIGFPE
//       occurs outside a guarded block. Use guarded blocks when you need exception translation.
//  3) You can disable later via DisableDivideByZeroTrapGlobal().
// ---------------------------

#undef DSC_THREAD_LOCAL