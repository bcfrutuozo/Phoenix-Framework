#pragma once

#include "System/Types.hpp"
#include "Win32Types.hpp"

class Control;
class EventQueue;
class UIContext;
class Window;
class IEventSink;

struct Win32ObjectHeader
{
    EventQueue* EventQueue;
    UIContext* Context;
    bool TrackingMouse = false;
    HWND Handle;
    IEventSink* EventSink;
};

struct NativeBackend : Win32ObjectHeader
{
    Control* Owner;
};