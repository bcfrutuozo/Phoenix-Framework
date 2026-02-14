#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "System/Input/MouseButton.hpp"
#include "GUI/Core/UIHandle.hpp"

class MouseMoveEvent final : public Event
{
public:

    MouseMoveEvent(UIHandle handle, Int32 x, Int32 y)
        :
        Event(handle, Category, Type),
        X(x),
        Y(y)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::Move);

    Int32 X;
    Int32 Y;
};

class MouseButtonDownEvent final : public Event
{
public:

    MouseButtonDownEvent(UIHandle handle, MouseButton button)
        :
        Event(handle, Category, Type),
        Button(button)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::ButtonDown);

    MouseButton Button;
};

class MouseButtonUpEvent final : public Event
{
public:

    MouseButtonUpEvent(UIHandle handle, MouseButton button)
        :
        Event(handle, Category, Type),
        Button(button)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::ButtonUp);

    MouseButton Button;
};

class MouseScrollEvent final : public Event
{
public:

    MouseScrollEvent(UIHandle handle, Single xoffset, Single yoffset)
        :
        Event(handle, Category, Type),
        XOffset(xoffset),
        YOffset(yoffset)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::Scroll);

    Single XOffset;
    Single YOffset;
};

class MouseEnterEvent final : public Event
{
public:

    MouseEnterEvent(UIHandle handle, Int32 x, Int32 y)
        :
        Event(handle, Category, Type),
        X(x),
        Y(y)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::Enter);

    Int32 X;
    Int32 Y;
};

class MouseLeaveEvent final : public Event
{
public:

    MouseLeaveEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::Mouse;
    static constexpr u8 Type = static_cast<uint8_t>(MouseEventType::Leave);
};