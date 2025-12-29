#pragma once

#include <cstdint>

enum class EventType : uint16_t
{
    Quit,
    WindowCreated,
    WindowCloseRequested,
    WindowResized,

    KeyDown,
    KeyUp,
    MouseMove,
    MouseButtonDown,
    MouseButtonUp,

    Timer,
    Custom
};