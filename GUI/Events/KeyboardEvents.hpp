#pragma once

#include "System/Events/Event.hpp"
#include "System/Input/KeyCode.hpp"
#include "System/Types.hpp"
#include "GUI/Window/WindowHandle.hpp"

class KeyDownEvent final : public Event
{
public:

    KeyDownEvent(UIHandle handle, KeyCode key, Boolean repeat)
        :
        Handle(handle),
        Key(key),
        Repeat(repeat)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Keyboard;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(KeyEventType::Down);
    }

    UIHandle Handle;
    KeyCode Key;
    Boolean Repeat;
};

class KeyUpEvent final : public Event
{
public:

    KeyUpEvent(UIHandle handle, KeyCode key)
        :
        Handle(handle),
        Key(key)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Keyboard;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(KeyEventType::Up);
    }

    UIHandle Handle;
    KeyCode Key;
};