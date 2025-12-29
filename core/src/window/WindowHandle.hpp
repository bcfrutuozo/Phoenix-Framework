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