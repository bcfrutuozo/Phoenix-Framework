#pragma once

#include "Events/Event.hpp"
#include "System/Types.hpp"

class SystemQuitEvent final : public Event
{
public:

	SystemQuitEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::Quit);
};

class SystemShutdownEvent final : public Event
{
public:

	SystemShutdownEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::Shutdown);
};

class SystemResumePowerEvent final : public Event
{
public:

	SystemResumePowerEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::PowerResume);
};

class SystemBatteryLowPowerEvent final : public Event
{
public:

	SystemBatteryLowPowerEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::PowerBatteryLow);
};

class SystemUnknownPowerEvent final : public Event
{
public:

	SystemUnknownPowerEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::PowerUnknown);
};

class SystemSuspendPowerEvent final : public Event
{
public:

	SystemSuspendPowerEvent(UIHandle handle)
		:
		Event(handle, Category, Type)
	{
		Flags = EventFlags::Propagable;
	}

	static constexpr EventCategory Category = EventCategory::System;
	static constexpr u8 Type = static_cast<uint8_t>(SystemEventType::PowerSuspend);
};