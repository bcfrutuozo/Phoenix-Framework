#pragma once

#include "System/Meta/TypeTraits.hpp"
#include "EventCategory.hpp"
#include "System/Types.hpp"
#include "System/time/TimePoint.hpp"
#include "GUI/Core/UIHandle.hpp"

enum class EventFlags : uint8_t
{
	None = 0,

	// 🔒 Default framework usage => Set to 0 to propagate further 
	Consumed = 1 << 0,

	// 👤 User suppress flag
	Handled = 1 << 1,

	// Is never consumed
	Propagable = 1 << 2,
};

constexpr EventFlags operator|(EventFlags a, EventFlags b)
{
	return static_cast<EventFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr EventFlags operator&(EventFlags a, EventFlags b)
{
	return static_cast<EventFlags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr EventFlags& operator|=(EventFlags& a, EventFlags b)
{
	a = a | b;
	return a;
}

class Event
{
public:

	inline constexpr Event(UIHandle handle, EventCategory category, u8 type) noexcept
		:
		Handle(handle),
		_category(category),
		_type(type)
	{
	}

	virtual ~Event() = default;

	inline constexpr EventCategory GetCategory() const noexcept { return _category; }
	inline constexpr u8 GetType() const noexcept { return _type; }

	template<typename T>
	inline const T& As() const noexcept
	{
		return static_cast<const T&>(*this);
	}

	inline constexpr Boolean Has(EventFlags f) const noexcept
	{
		return (Flags & f) != EventFlags::None;
	}

	inline constexpr void Set(EventFlags f) noexcept
	{
		Flags |= f;
	}

	inline constexpr void Clear(EventFlags f) noexcept
	{
		Flags = static_cast<EventFlags>(static_cast<uint8_t>(Flags) & ~static_cast<uint8_t>(f));
	}

	TimePoint Timestamp;
	EventFlags Flags = EventFlags::None;
	UIHandle Handle;

private:

	EventCategory _category;
	u8 _type;
};