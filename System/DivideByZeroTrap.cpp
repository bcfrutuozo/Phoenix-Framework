#include "DivideByZeroTrap.hpp"
#include "Exceptions.hpp"

#include <stdint.h>
#include <setjmp.h>

#if defined(_WIN32) || defined(_WIN64)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <float.h>
#include <eh.h>

#else

#include <signal.h>
#include <fenv.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ucontext.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#endif

// ---------------------------------------------------------
// Thread-local state
// ---------------------------------------------------------

#if defined(_MSC_VER)
#define DSC_THREAD_LOCAL __declspec(thread)
#else
#define DSC_THREAD_LOCAL __thread
#endif

#if defined(_WIN32) || defined(_WIN64)
typedef jmp_buf dsc_sigjmp_buf;
#define dsc_sigsetjmp(env, save) setjmp(env)
#define dsc_siglongjmp(env, val) longjmp(env, val)
#else
typedef sigjmp_buf dsc_sigjmp_buf;
#define dsc_sigsetjmp(env, save) sigsetjmp(env, save)
#define dsc_siglongjmp(env, val) siglongjmp(env, val)
#endif

static DSC_THREAD_LOCAL dsc_sigjmp_buf* g_currentJmpBuf = nullptr;

// ---------------------------------------------------------
// Global trap state
// ---------------------------------------------------------

static struct
{
#if defined(_WIN32) || defined(_WIN64)
    _se_translator_function prevTranslator;
#else
    struct sigaction prevSigAction;
    int fpTrapsEnabled;
#endif
    int installed;
} g_trapState = { 0 };

// ---------------------------------------------------------
// POSIX handler
// ---------------------------------------------------------

#if !defined(_WIN32) && !defined(_WIN64)

static void SigFpeHandler(int sig, siginfo_t* info, void* ctx)
{
    (void)info; (void)ctx;

    if (sig != SIGFPE)
        return;

    if (g_currentJmpBuf)
        dsc_siglongjmp(*g_currentJmpBuf, 1);

    _exit(128 + SIGFPE);
}

#endif

// ---------------------------------------------------------
// Windows SEH translator
// ---------------------------------------------------------

#if defined(_WIN32) || defined(_WIN64)

static void __cdecl SehTranslator(unsigned int code, struct _EXCEPTION_POINTERS*)
{
    if (code == EXCEPTION_INT_DIVIDE_BY_ZERO ||
        code == EXCEPTION_FLT_DIVIDE_BY_ZERO)
    {
        throw DivideByZeroException();
    }

    throw InvalidOperationException("Unhandled structured exception");
}

#endif

// ---------------------------------------------------------
// Floating-point trap control
// ---------------------------------------------------------

static void EnableFPTraps()
{
#if defined(_WIN32) || defined(_WIN64)
    unsigned int cw;
    _controlfp_s(&cw, 0, 0);
    _controlfp_s(&cw, cw & ~_EM_ZERODIVIDE, _MCW_EM);
#else
#ifdef FE_DIVBYZERO
    feenableexcept(FE_DIVBYZERO);
#endif
#endif
}

static void DisableFPTraps()
{
#if defined(_WIN32) || defined(_WIN64)
    unsigned int cw;
    _controlfp_s(&cw, _EM_ZERODIVIDE, _MCW_EM);
#else
#if defined(__GLIBC__) && defined(FE_DIVBYZERO)
    fedisableexcept(FE_DIVBYZERO);
#endif
#endif
}

// ---------------------------------------------------------
// Public API
// ---------------------------------------------------------

int EnableDivideByZeroTrapGlobal(int enableFloatTraps)
{
    if (g_trapState.installed)
        return 0;

#if defined(_WIN32) || defined(_WIN64)

    g_trapState.prevTranslator = nullptr;
    _set_se_translator(SehTranslator);

    if (enableFloatTraps)
        EnableFPTraps();

#else

    struct sigaction act {};
    act.sa_sigaction = SigFpeHandler;
    act.sa_flags = SA_SIGINFO | SA_NODEFER;

    if (sigaction(SIGFPE, &act, &g_trapState.prevSigAction) != 0)
        return -1;

    if (enableFloatTraps)
        EnableFPTraps();

#endif

    g_trapState.installed = 1;
    return 0;
}

void DisableDivideByZeroTrapGlobal()
{
    if (!g_trapState.installed)
        return;

#if defined(_WIN32) || defined(_WIN64)
    _set_se_translator(nullptr);
    DisableFPTraps();
#else
    sigaction(SIGFPE, &g_trapState.prevSigAction, nullptr);
    DisableFPTraps();
#endif

    g_trapState.installed = 0;
}

// ---------------------------------------------------------
// Guarded execution
// ---------------------------------------------------------

template<typename Callable>
void RunWithDivideByZeroGuard(Callable&& fn)
{
    dsc_sigjmp_buf buf;
    g_currentJmpBuf = &buf;

    if (dsc_sigsetjmp(buf, 1) == 0)
    {
        try { fn(); }
        catch (...)
        {
            g_currentJmpBuf = nullptr;
            throw;
        }
        g_currentJmpBuf = nullptr;
    }
    else
    {
        g_currentJmpBuf = nullptr;
        throw DivideByZeroException();
    }
}

template<typename Callable>
int TryRunCatchDivideByZero(Callable&& fn)
{
    try
    {
        RunWithDivideByZeroGuard(fn);
        return 0;
    }
    catch (const DivideByZeroException&)
    {
        return 1;
    }
}