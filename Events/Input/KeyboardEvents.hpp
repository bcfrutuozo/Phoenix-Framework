#pragma once

#include "Events/Event.hpp"
#include "System/Input/KeyCode.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class KeyDownEvent final : public Event
{
public:

    KeyDownEvent(UIHandle handle, KeyCode key, Boolean repeat)
        :
        Event(handle, Category, Type),
        Key(key),
        Repeat(repeat)
    {
    }

    static constexpr EventCategory Category = EventCategory::Keyboard;
    static constexpr u8 Type = static_cast<uint8_t>(KeyEventType::Down);

    KeyCode Key;
    Boolean Repeat;
};

class KeyUpEvent final : public Event
{
public:

    KeyUpEvent(UIHandle handle, KeyCode key)
        :
        Event(handle, Category, Type),
        Key(key)
    {
    }

    static constexpr EventCategory Category = EventCategory::Keyboard;
    static constexpr u8 Type = static_cast<uint8_t>(KeyEventType::Up);

    KeyCode Key;
};