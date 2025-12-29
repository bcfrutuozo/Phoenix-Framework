#pragma once

#include "Event.hpp"
#include "types/Int32.hpp"
#include "types/Single.hpp"
#include "input/MouseButton.hpp"
#include "window/WindowHandle.hpp"

class MouseMoveEvent final : public Event
{
public:

    MouseMoveEvent(WindowHandle handle, Int32 x, Int32 y)
        :
        Window(handle),
        X(x),
        Y(y)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Mouse;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(MouseEventType::Move);
    }

    WindowHandle Window;
    Int32 X;
    Int32 Y;
};

class MouseButtonDownEvent final : public Event
{
public:

    MouseButtonDownEvent(WindowHandle handle, MouseButton button)
        :
        Window(handle),
        Button(button)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Mouse;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(MouseEventType::ButtonDown);
    }

    WindowHandle Window;
    MouseButton Button;
};

class MouseButtonUpEvent final : public Event
{
public:

    MouseButtonUpEvent(WindowHandle handle, MouseButton button)
        :
        Window(handle),
        Button(button)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Mouse;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(MouseEventType::ButtonUp);
    }

    WindowHandle Window;
    MouseButton Button;
};

class MouseScrollEvent final : public Event
{
public:

    MouseScrollEvent(WindowHandle handle, Single xoffset, Single yoffset)
        :
        Window(handle),
        XOffset(xoffset),
        YOffset(yoffset)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Mouse;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(MouseEventType::Scroll);
    }

    WindowHandle Window;
    Single XOffset;
    Single YOffset;
};