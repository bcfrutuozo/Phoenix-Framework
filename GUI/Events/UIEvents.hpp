#pragma once

#include "System/Events/Event.hpp"
#include "System/Types.hpp"
#include "GUI/Window/WindowHandle.hpp"

class UIResizeEvent final: public Event
{
public:

    UIResizeEvent(UIHandle handle, Int32 w, Int32 h)
        :
        Handle(handle),
        Width(w),
        Height(h)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Resize);
    }

    UIHandle Handle;
    Int32 Width;
    Int32 Height;
};

class UIMinimizeEvent final : public Event
{
public:

    UIMinimizeEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Minimize);
    }

    UIHandle Handle;
};

class UIMaximizeEvent final : public Event
{
public:

    UIMaximizeEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Maximize);
    }

    UIHandle Handle;
};

class UICloseEvent final : public Event
{
public:

    UICloseEvent(UIHandle handle)
        :
        Handle(handle)
    {}

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Close);
    }

    UIHandle Handle;
};

class UIDestroyEvent final : public Event
{
public:

    UIDestroyEvent(UIHandle handle)
        :
        Handle(handle)
    { }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Destroy);
    }

    UIHandle Handle;
};

class UIMoveEvent final : public Event
{
public:

    UIMoveEvent(UIHandle handle, Int32 x, Int32 y)
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

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Move);
    }

    UIHandle Handle;
    Int32 X;
    Int32 Y;
};

class UIFocusGainedEvent final : public Event
{
public:

    UIFocusGainedEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::FocusGained);
    }

    UIHandle Handle;
};

class UIFocusLostEvent final : public Event
{
public:

    UIFocusLostEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::FocusLost);
    }

    UIHandle Handle;
};

class UIShowEvent final : public Event
{
public:

    UIShowEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Show);
    }

    UIHandle Handle;
};

class UIHideEvent final : public Event
{
public:

    UIHideEvent(UIHandle handle)
        :
        Handle(handle)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::Hide);
    }

    UIHandle Handle;
};

class UIDPIChangedEvent final : public Event
{
public:

    UIDPIChangedEvent(UIHandle handle, UInt32 dpi)
        :
        Handle(handle),
        DPI(dpi)
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::UI;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(UIEventType::DPIChanged);
    }

    UIHandle Handle;
    UInt32 DPI;
};