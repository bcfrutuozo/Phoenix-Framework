#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "System/Input/MouseButton.hpp"
#include "GUI/Window/WindowHandle.hpp"

class MouseMoveEvent final : public Event
{
public:

    MouseMoveEvent(UIHandle handle, Int32 x, Int32 y)
        :
        Handle(handle),
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

    UIHandle Handle;
    Int32 X;
    Int32 Y;
};

class MouseButtonDownEvent final : public Event
{
public:

    MouseButtonDownEvent(UIHandle handle, MouseButton button)
        :
        Handle(handle),
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

    UIHandle Handle;
    MouseButton Button;
};

class MouseButtonUpEvent final : public Event
{
public:

    MouseButtonUpEvent(UIHandle handle, MouseButton button)
        :
        Handle(handle),
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

    UIHandle Handle;
    MouseButton Button;
};

class MouseScrollEvent final : public Event
{
public:

    MouseScrollEvent(UIHandle handle, Single xoffset, Single yoffset)
        :
        Handle(handle),
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

    UIHandle Handle;
    Single XOffset;
    Single YOffset;
};