#pragma once

#include "System/Types.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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