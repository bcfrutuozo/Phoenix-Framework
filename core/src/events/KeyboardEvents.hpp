#pragma once

#include "Event.hpp"
#include "input/KeyCode.hpp"
#include "types/Boolean.hpp"
#include "window/WindowHandle.hpp"

class KeyDownEvent final : public Event
{
public:

    KeyDownEvent(WindowHandle handle, KeyCode key, Boolean repeat)
        :
        Window(handle),
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

    WindowHandle Window;
    KeyCode Key;
    Boolean Repeat;
};

class KeyUpEvent final : public Event
{
public:

    KeyUpEvent(WindowHandle handle, KeyCode key)
        :
        Window(handle),
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

    WindowHandle Window;
    KeyCode Key;
};