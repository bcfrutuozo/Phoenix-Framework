#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"

class SystemQuitEvent final : public Event
{
public:

	SystemQuitEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(SystemEventType::Quit);
	}
};

class SystemShutdownEvent final : public Event
{
public:

	SystemShutdownEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(SystemEventType::Shutdown);
	}
};

class SystemResumePowerEvent final : public Event
{
public:

	SystemResumePowerEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(PowerEventType::Resume);
	}
};

class SystemBatteryLowPowerEvent final : public Event
{
public:

	SystemBatteryLowPowerEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(PowerEventType::BatteryLow);
	}
};

class SystemUnknownPowerEvent final : public Event
{
public:

	SystemUnknownPowerEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(PowerEventType::Unknown);
	}
};

class SystemSuspendPowerEvent final : public Event
{
public:

	SystemSuspendPowerEvent()
	{
		Flags = EventFlags::Propagable;
	}

	EventCategory Category() const noexcept override
	{
		return EventCategory::System;
	}

	u8 TypeId() const noexcept override
	{
		return static_cast<uint8_t>(PowerEventType::Suspend);
	}
};