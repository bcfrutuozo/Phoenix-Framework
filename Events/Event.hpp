#pragma once

#include "System/Meta/TypeTraits.hpp"
#include "EventCategory.hpp"
#include "System/Types.hpp"
#include "System/time/TimePoint.hpp"

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

	virtual ~Event() = default;

	virtual EventCategory Category() const noexcept = 0;
	virtual u8 TypeId() const noexcept = 0;

	Boolean Is(EventCategory cat) const noexcept
	{
		return (Category() & cat) != EventCategory::None;
	}

	template<typename TEnum>
		requires(is_enum_v<TEnum>)
	constexpr TEnum Type() const noexcept
	{
		using U = underlying_type_t<TEnum>;
		static_assert(sizeof(typename decltype(TypeId())::value_type) >= sizeof(U), "TypeId wrapper too small for this enum");
		return static_cast<TEnum>(static_cast<U>(TypeId()));
	}

	template<typename T>
	const T& As() const noexcept
	{
		return static_cast<const T&>(*this);
	}

	Boolean Has(EventFlags f) const noexcept
	{
		return (Flags & f) != EventFlags::None;
	}

	void Set(EventFlags f) noexcept
	{
		Flags |= f;
	}

	void Clear(EventFlags f) noexcept
	{
		Flags = static_cast<EventFlags>(static_cast<uint8_t>(Flags) & ~static_cast<uint8_t>(f));
	}

	TimePoint Timestamp;
	EventFlags Flags = EventFlags::None;
};