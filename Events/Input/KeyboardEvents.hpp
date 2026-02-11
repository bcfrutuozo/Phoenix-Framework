#pragma once

#include "Events/Event.hpp"
#include "System/Input/KeyCode.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class KeyDownEvent final : public Event
{
public:

    KeyDownEvent(UIHandle target, KeyCode key, Boolean repeat)
        :
        Target(target),
        Key(key),
        Repeat(repeat)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Keyboard;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(KeyEventType::Down);
    }

    UIHandle Target;
    KeyCode Key;
    Boolean Repeat;
};

class KeyUpEvent final : public Event
{
public:

    KeyUpEvent(UIHandle target, KeyCode key)
        :
        Target(target),
        Key(key)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::Keyboard;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(KeyEventType::Up);
    }

    UIHandle Target;
    KeyCode Key;
};