#pragma once

#include "types/Pointer.hpp"

struct WindowNativeHandle
{
    Pointer value = nullptr;
};

struct DisplayNativeHandle
{
    Pointer value = nullptr;
};

struct WindowHandle
{
    WindowNativeHandle  window;
    DisplayNativeHandle display;
};

// ------------------------------------------------------------
// Equality
// ------------------------------------------------------------
inline Boolean operator==(const WindowHandle& a, const WindowHandle& b) noexcept
{
    return (a.window.value == b.window.value) &&
        (a.display.value == b.display.value);
}

inline Boolean operator!=(const WindowHandle& a, const WindowHandle& b) noexcept
{
    return !(a == b);
}