#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "System/Input/MouseButton.hpp"
#include "GUI/Core/UIHandle.hpp"

class MouseEnterEvent final : public Event
{
public:

    MouseEnterEvent(UIHandle target, Int32 x, Int32 y)
        :
        Target(target),
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
        return static_cast<uint32_t>(MouseEventType::Enter);
    }

    UIHandle Target;
    Int32 X;
    Int32 Y;
};

class MouseLeaveEvent final : public Event
{
public:

    MouseLeaveEvent(UIHandle target)
        :
        Target(target)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Mouse;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(MouseEventType::Leave);
    }

    UIHandle Target;
};

class MouseMoveEvent final : public Event
{
public:

    MouseMoveEvent(UIHandle target, Int32 x, Int32 y)
        :
        Target(target),
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

    UIHandle Target;
    Int32 X;
    Int32 Y;
};

class MouseButtonDownEvent final : public Event
{
public:

    MouseButtonDownEvent(UIHandle target, MouseButton button)
        :
        Target(target),
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

    UIHandle Target;
    MouseButton Button;
};

class MouseButtonUpEvent final : public Event
{
public:

    MouseButtonUpEvent(UIHandle target, MouseButton button)
        :
        Target(target),
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

    UIHandle Target;
    MouseButton Button;
};

class MouseScrollEvent final : public Event
{
public:

    MouseScrollEvent(UIHandle target, Single xoffset, Single yoffset)
        :
        Target(target),
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

    UIHandle Target;
    Single XOffset;
    Single YOffset;
};