#pragma once

#include "System/Types.hpp"
#include "Win32Types.hpp"

class Control;
class EventQueue;
class UIContext;
class Window;

struct Win32ObjectHeader
{
    EventQueue* queue;
    UIContext* context;
    bool trackingMouse = false;
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