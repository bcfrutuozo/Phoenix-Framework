#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"

class SystemQuitEvent final : public Event
{
public:

    SystemQuitEvent() = default;

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(SystemEventType::Quit);
    }
};

class SystemShutdownEvent final : public Event
{
public:

    SystemShutdownEvent() = default;

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(SystemEventType::Shutdown);
    }
};

class SystemResumePowerEvent final : public Event
{
public:

    SystemResumePowerEvent()
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(PowerEventType::Resume);
    }
};

class SystemBatteryLowPowerEvent final : public Event
{
public:

    SystemBatteryLowPowerEvent()
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(PowerEventType::BatteryLow);
    }
};

class SystemUnknownPowerEvent final : public Event
{
public:

    SystemUnknownPowerEvent()
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(PowerEventType::Unknown);
    }
};

class SystemSuspendPowerEvent final : public Event
{
public:

    SystemSuspendPowerEvent()
    {
    }

    EventCategory Category() const noexcept override
    {
        return EventCategory::System;
    }

    UInt32 TypeId() const noexcept override
    {
        return static_cast<uint32_t>(PowerEventType::Suspend);
    }
};