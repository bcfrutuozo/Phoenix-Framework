#pragma once

#include <cstdint>

enum class GamepadButton : uint16_t
{
    None = 0,

    // Face buttons
    A,
    B,
    X,
    Y,

    // Shoulder buttons
    LeftShoulder,
    RightShoulder,

    // Stick buttons
    LeftStick,
    RightStick,

    // System buttons
    Back,
    Start,
    Guide,      // Xbox button / PS button

    // D-Pad
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,

    // Optional / newer controllers
    Misc1,      // Share / Capture
    Paddle1,
    Paddle2,
    Paddle3,
    Paddle4,

    Touchpad    // PS controllers
};