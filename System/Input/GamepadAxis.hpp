#pragma once

#include <cstdint>

enum class GamepadAxis : uint16_t
{
    None = 0,

    // Left stick
    LeftX,
    LeftY,

    // Right stick
    RightX,
    RightY,

    // Triggers
    LeftTrigger,
    RightTrigger
};