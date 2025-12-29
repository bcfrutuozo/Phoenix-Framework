#pragma once

#include "types/Pointer.hpp"

struct WindowNativeHandle
{
    Pointer value;
};

struct DisplayNativeHandle
{
    Pointer value;
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