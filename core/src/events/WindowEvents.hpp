#pragma once

#include "Event.hpp"
#include "types/Int32.hpp"
#include "window/WindowHandle.hpp"

class WindowResizeEvent final: public Event
{
public:

    WindowResizeEvent(WindowHandle handle, Int32 w, Int32 h)
        :
        Window(handle),
        Width(w),
        Height(h)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Resize);
    }

    WindowHandle Window;
    Int32 Width;
    Int32 Height;
};

class WindowMinimizeEvent final : public Event
{
public:

    WindowMinimizeEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Minimize);
    }

    WindowHandle Window;
};

class WindowMaximizeEvent final : public Event
{
public:

    WindowMaximizeEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Maximize);
    }

    WindowHandle Window;
};

class WindowCloseEvent final : public Event
{
public:

    WindowCloseEvent(WindowHandle handle)
        :
        Window(handle)
    {}

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Close);
    }

    WindowHandle Window;
};

class WindowDestroyEvent final : public Event
{
public:

    WindowDestroyEvent(WindowHandle handle)
        :
        Window(handle)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Destroy);
    }

    WindowHandle Window;
};

class WindowMoveEvent final : public Event
{
public:

    WindowMoveEvent(WindowHandle handle, Int32 x, Int32 y)
        :
        Window(handle),
        X(x),
        Y(y)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Move);
    }

    WindowHandle Window;
    Int32 X;
    Int32 Y;
};

class WindowFocusGainedEvent final : public Event
{
public:

    WindowFocusGainedEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::FocusGained);
    }

    WindowHandle Window;
};

class WindowFocusLostEvent final : public Event
{
public:

    WindowFocusLostEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::FocusLost);
    }

    WindowHandle Window;
};

class WindowShowEvent final : public Event
{
public:

    WindowShowEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Show);
    }

    WindowHandle Window;
};

class WindowHideEvent final : public Event
{
public:

    WindowHideEvent(WindowHandle handle)
        :
        Window(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::Hide);
    }

    WindowHandle Window;
};

class WindowDPIChangedEvent final : public Event
{
public:

    WindowDPIChangedEvent(WindowHandle handle, UInt32 dpi)
        :
        Window(handle),
        DPI(dpi)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Window;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(WindowEventType::DPIChanged);
    }

    WindowHandle Window;
    UInt32 DPI;
};