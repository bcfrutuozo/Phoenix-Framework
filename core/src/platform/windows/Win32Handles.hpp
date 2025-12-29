#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "window/WindowHandle.hpp"

inline HWND ToHWND(WindowNativeHandle h) noexcept
{
    return static_cast<HWND>(h.value.Get());
}

inline HINSTANCE ToHINSTANCE(DisplayNativeHandle h) noexcept
{
    return static_cast<HINSTANCE>(h.value.Get());
}