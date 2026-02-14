#pragma once

#include "Events/Event.hpp"
#include "System/Input/GamepadButton.hpp"
#include "System/Input/GamepadAxis.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class GamepadConnectedEvent final : public Event
{
public:

    GamepadConnectedEvent(UIHandle handle, u32 gamepadId)
        :
        Event(handle, Category, Type),
        GamepadId(gamepadId)
    {
    }

    static constexpr EventCategory Category = EventCategory::Gamepad;
    static constexpr u8 Type = static_cast<uint8_t>(GamepadEventType::Connected);

    u32 GamepadId;
};

class GamepadDisconnectedEvent final : public Event
{
public:

    GamepadDisconnectedEvent(UIHandle handle, u32 gamepadId)
        :
        Event(handle, Category, Type),
        GamepadId(gamepadId)
    {
    }

    static constexpr EventCategory Category = EventCategory::Gamepad;
    static constexpr u8 Type = static_cast<uint8_t>(GamepadEventType::Disconnected);

    u32 GamepadId;
};

class GamepadButtonDownEvent final : public Event
{
public:

    GamepadButtonDownEvent(UIHandle handle, u32 gamepadId, GamepadButton button)
        :
        Event(handle, Category, Type),
        GamepadId(gamepadId),
        Button(button)
    {
    }

    static constexpr EventCategory Category = EventCategory::Gamepad;
    static constexpr u8 Type = static_cast<uint8_t>(GamepadEventType::ButtonDown);
 
    u32 GamepadId;
    GamepadButton Button;
};

class GamepadButtonUpEvent final : public Event
{
public:

    GamepadButtonUpEvent(UIHandle handle, u32 gamepadId, GamepadButton button)
        :
        Event(handle, Category, Type),
        GamepadId(gamepadId),
        Button(button)
    {
    }

    static constexpr EventCategory Category = EventCategory::Gamepad;
    static constexpr u8 Type = static_cast<uint8_t>(GamepadEventType::ButtonUp);

    u32 GamepadId;
    GamepadButton Button;
};

class GamepadAxisMoveEvent final : public Event
{
public:

    GamepadAxisMoveEvent(UIHandle handle, u32 gamepadId, GamepadAxis axix, Single value)
        :
        Event(handle, Category, Type),
        GamepadId(gamepadId),
        Value(value)
    {
    }

    static constexpr EventCategory Category = EventCategory::Gamepad;
    static constexpr u8 Type = static_cast<uint8_t>(GamepadEventType::AxisMove);

    u32 GamepadId;
    GamepadAxis Axis;
    Single Value;
};