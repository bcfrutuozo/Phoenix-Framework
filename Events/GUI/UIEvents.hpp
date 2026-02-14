#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class Font;

class CloseEvent final : public Event
{
public:

    CloseEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Close);
};

class ResizedEvent final: public Event
{
public:

    ResizedEvent(UIHandle handle, Int32 w, Int32 h)
        :
        Event(handle, Category, Type),
        Width(w),
        Height(h)
    { }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Resized);

    Int32 Width;
    Int32 Height;
};

class ResizingEvent final : public Event
{
public:

    ResizingEvent(UIHandle handle, Int32 w, Int32 h)
        :
        Event(handle, Category, Type),
        Width(w),
        Height(h)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Resizing);

    Int32 Width;
    Int32 Height;
};

class MinimizeEvent final : public Event
{
public:

    MinimizeEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Minimize);
};

class MaximizeEvent final : public Event
{
public:

    MaximizeEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Maximize);
};

class RestoreEvent final : public Event
{
public:

    RestoreEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Restore);
};

class DestroyEvent final : public Event
{
public:

    DestroyEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    { }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Destroy);
};

class MoveEvent final : public Event
{
public:

    MoveEvent(UIHandle handle, Int32 x, Int32 y)
        :
        Event(handle, Category, Type),
        X(x),
        Y(y)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Move);

    Int32 X;
    Int32 Y;
};

class FocusGainedEvent final : public Event
{
public:

    FocusGainedEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::FocusGained);
};

class FocusLostEvent final : public Event
{
public:

    FocusLostEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::FocusLost);
};

class ShowEvent final : public Event
{
public:

    ShowEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Show);
};

class HideEvent final : public Event
{
public:

    HideEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Hide);
};

class DPIChangedEvent final : public Event
{
public:

    DPIChangedEvent(UIHandle handle, UInt32 dpi)
        :
        Event(handle, Category, Type),
        DPI(dpi)
    {
        Flags = EventFlags::Propagable;
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::DPIChanged);

    UInt32 DPI;
};

class PaintEvent final : public Event
{
public:

    PaintEvent(UIHandle handle)
        :
        Event(handle, Category, Type)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::Paint);
};

class FontChangedEvent final : public Event
{
public:

    FontChangedEvent(UIHandle handle, Font* newFont, Font* previousFont)
        :
        Event(handle, Category, Type),
        NewFont(newFont),
        PreviousFont(previousFont)
    {
    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::FontChanged);

    Font* NewFont;
    Font* PreviousFont;
};

class ControlAddedEvent final : public Event
{
public:

    ControlAddedEvent(UIHandle handle, UIHandle addedControl)
        :
        Event(handle, Category, Type),
        Added(addedControl)
    {

    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::ControlAdded);

    UIHandle Added;
};

class ControlRemovedEvent final : public Event
{
public:

    ControlRemovedEvent(UIHandle handle, UIHandle removedControl)
        :
        Event(handle, Category, Type),
        Removed(removedControl)
    {

    }

    static constexpr EventCategory Category = EventCategory::UI;
    static constexpr u8 Type = static_cast<uint8_t>(UIEventType::ControlRemoved);

    UIHandle Removed;
};