#pragma once

#include <cstdint>

enum class EventCategory : uint8_t
{
    UI,
    Keyboard,
    Mouse,
    Touch,
    Gamepad,
    Text,
    Clipboard,
    System,
};

enum class UIEventType : uint8_t
{
    Close,
    Destroy,
    Resized,
    Resizing,
    Move,
    Restore,
    Minimize,
    Maximize,
    FocusGained,
    FocusLost,
    Show,
    Hide,
    DPIChanged,
    FontChanged,
    Paint,
    ControlAdded,
    ControlRemoved
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
    Enter,
    Leave
};

enum class TouchEventType : uint8_t
{
    Move,
    Down,
    Up
};

enum class GamepadEventType : uint8_t
{
    Connected,
    Disconnected,
    ButtonDown,
    ButtonUp,
    AxisMove
};

enum class TextEventType : uint8_t
{
    Input,
    ImeCompositionStart,
    ImeCompositionUpdate,
    ImeCompositionEnd,
    ImeCompositionCommit,
};

enum class ClipboardEventType : uint8_t
{
    Copy,
    Paste,
    Cut,
    Changed
};

enum class SystemEventType : uint8_t
{
    Quit,
    Shutdown,
    PowerSuspend,
    PowerResume,
    PowerBatteryLow,
    PowerUnknown
};