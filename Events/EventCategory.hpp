#pragma once

#include <cstdint>

enum class EventCategory : uint16_t
{
    None = 0,
    UI = 1 << 0,
    Keyboard = 1 << 1,
    Mouse = 1 << 2,
    Touch = 1 << 3,
    Gamepad = 1 << 4,
    Text = 1 << 5,
    Clipboard = 1 << 6,
    System = 1 << 7,
};

enum class UIEventType : uint8_t
{
    Close,
    Destroy,
    Resize,
    Move,
    Restore,
    Minimize,
    Maximize,
    FocusGained,
    FocusLost,
    Show,
    Hide,
    DPIChanged
};

enum class KeyEventType : uint8_t
{
    Down,
    Up,
};

enum class MouseEventType : uint8_t
{
    Move,
    ButtonDown,
    ButtonUp,
    Scroll,
};

enum class TextEventType : uint8_t
{
    Input,
    ImeComposition
};

enum class SystemEventType : uint8_t
{
    Quit,
    Shutdown
};

enum class PowerEventType : uint8_t
{
    Suspend,
    Resume,
    BatteryLow,
    Unknown
};


constexpr EventCategory operator|(EventCategory a, EventCategory b) noexcept
{
    return static_cast<EventCategory>(
        static_cast<uint16_t>(a) | static_cast<uint16_t>(b)
        );
}

constexpr EventCategory operator&(EventCategory a, EventCategory b) noexcept
{
    return static_cast<EventCategory>(
        static_cast<uint16_t>(a) & static_cast<uint16_t>(b)
        );
}

constexpr EventCategory operator~(EventCategory a) noexcept
{
    return static_cast<EventCategory>(
        ~static_cast<uint16_t>(a)
        );
}