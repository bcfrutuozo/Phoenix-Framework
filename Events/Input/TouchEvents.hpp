#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"

class TouchMoveEvent final : public Event
{
public:

    TouchMoveEvent(UIHandle handle, u32 fingerId, Single x, Single y, Single deltaX, Single deltaY)
        :
        Event(handle, Category, Type),
        FingerId(fingerId),
        X(x),
        Y(y),
        DeltaX(deltaX),
        DeltaY(deltaY)
    {
    }

    static constexpr EventCategory Category = EventCategory::Touch;
    static constexpr u8 Type = static_cast<uint8_t>(TouchEventType::Move);

    u32 FingerId;
    Single X;
    Single Y;
    Single DeltaX;
    Single DeltaY;
};

class TouchDownEvent final : public Event
{
public:

    TouchDownEvent(UIHandle handle, u32 fingerId, Single x, Single y, Single pressure)
        :
        Event(handle, Category, Type),
        FingerId(fingerId),
        X(x),
        Y(y),
        Pressure(pressure)
    {
    }

    static constexpr EventCategory Category = EventCategory::Touch;
    static constexpr u8 Type = static_cast<uint8_t>(TouchEventType::Down);

    u32 FingerId;
    Single X;
    Single Y;
    Single Pressure;
};

class TouchUpEvent final : public Event
{
public:

    TouchUpEvent(UIHandle handle, u32 fingerId, Single x, Single y)
        :
        Event(handle, Category, Type),
        FingerId(fingerId),
        X(x),
        Y(y)
    {
    }

    static constexpr EventCategory Category = EventCategory::Touch;
    static constexpr u8 Type = static_cast<uint8_t>(TouchEventType::Up);

    u32 FingerId;
    Single X;
    Single Y;
};