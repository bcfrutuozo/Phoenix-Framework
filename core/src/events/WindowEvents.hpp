#pragma once

#include "Event.hpp"
#include "window/WindowHandle.hpp"

struct WindowResizeEvent : Event
{
    WindowResizeEvent(WindowHandle handle, Int32 w, Int32 h)
        :
        window(handle),
        width(w),
        height(h)
    { }

    WindowHandle window;
    Int32 width;
    Int32 height;
};

struct WindowCloseEvent : Event
{
    WindowCloseEvent(WindowHandle handle)
        :
        window(handle)
    {}

    WindowHandle window;
};