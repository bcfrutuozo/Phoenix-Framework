#pragma once

#include "System/Types.hpp"
#include "Win32Types.hpp"

class Control;
class EventQueue;
class Window;

enum class Win32ObjectKind : uint8_t
{
    Window,
    Control
};

struct Win32ObjectHeader
{
    Win32ObjectKind Kind;
    EventQueue* queue;
    HWND hwnd;
};

struct ControlBackend : Win32ObjectHeader
{
    Control* owner;
};

struct LabelBackend : ControlBackend
{

};

struct WindowBackend : Win32ObjectHeader
{
    Window* owner;
};