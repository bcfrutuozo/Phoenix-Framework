#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

class Font;

class ResizedEvent final: public Event
{
public:

    ResizedEvent(UIHandle handle, Int32 w, Int32 h)
        :
        Handle(handle),
        Width(w),
        Height(h)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Resized);
    }

    UIHandle Handle;
    Int32 Width;
    Int32 Height;
};

class ResizingEvent final : public Event
{
public:

    ResizingEvent(UIHandle handle, Int32 w, Int32 h)
        :
        Handle(handle),
        Width(w),
        Height(h)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Resizing);
    }

    UIHandle Handle;
    Int32 Width;
    Int32 Height;
};

class MinimizeEvent final : public Event
{
public:

    MinimizeEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Minimize);
    }

    UIHandle Handle;
};

class MaximizeEvent final : public Event
{
public:

    MaximizeEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Maximize);
    }

    UIHandle Handle;
};

class RestoreEvent final : public Event
{
public:

    RestoreEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Restore);
    }

    UIHandle Handle;
};

class CloseEvent final : public Event
{
public:

    CloseEvent(UIHandle handle)
        :
        Handle(handle)
    {}

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Close);
    }

    UIHandle Handle;
};

class DestroyEvent final : public Event
{
public:

    DestroyEvent(UIHandle handle)
        :
        Handle(handle)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Destroy);
    }

    UIHandle Handle;
};

class MoveEvent final : public Event
{
public:

    MoveEvent(UIHandle handle, Int32 x, Int32 y)
        :
        Handle(handle),
        X(x),
        Y(y)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Move);
    }

    UIHandle Handle;
    Int32 X;
    Int32 Y;
};

class FocusGainedEvent final : public Event
{
public:

    FocusGainedEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::FocusGained);
    }

    UIHandle Handle;
};

class FocusLostEvent final : public Event
{
public:

    FocusLostEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::FocusLost);
    }

    UIHandle Handle;
};

class ShowEvent final : public Event
{
public:

    ShowEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Show);
    }

    UIHandle Handle;
};

class HideEvent final : public Event
{
public:

    HideEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Hide);
    }

    UIHandle Handle;
};

class DPIChangedEvent final : public Event
{
public:

    DPIChangedEvent(UIHandle handle, UInt32 dpi)
        :
        Handle(handle),
        DPI(dpi)
    {
        Flags = EventFlags::Propagable;
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::DPIChanged);
    }

    UIHandle Handle;
    UInt32 DPI;
};

class PaintEvent final : public Event
{
public:

    PaintEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::Paint);
    }

    UIHandle Handle;
};

class FontChangedEvent final : public Event
{
public:

    FontChangedEvent(UIHandle handle, Font* newFont, Font* previousFont)
        :
        Target(handle),
        NewFont(newFont),
        PreviousFont(previousFont)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::DPIChanged);
    }

    UIHandle Target;
    Font* NewFont;
    Font* PreviousFont;
};

class ControlAddedEvent final : public Event
{
public:

    ControlAddedEvent(UIHandle handle, UIHandle addedControl)
        :
        Target(handle),
        Child(addedControl)
    {

    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::ControlAdded);
    }

    UIHandle Target;
    UIHandle Child;
};

class ControlRemovedEvent final : public Event
{
public:

    ControlRemovedEvent(UIHandle handle, UIHandle removedControl)
        :
        Target(handle),
        Child(removedControl)
    {

    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    u8 TypeId() const noexcept override
    {
        return static_cast<uint8_t>(UIEventType::ControlRemoved);
    }

    UIHandle Target;
    UIHandle Child;
};