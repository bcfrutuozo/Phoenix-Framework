#pragma once

#include "System/Meta/TypeTraits.hpp"
#include "EventCategory.hpp"
#include "System/Types.hpp"
#include "System/time/TimePoint.hpp"

class Event
{
public:

    virtual ~Event() = default;

    virtual EventCategory Category() const noexcept = 0;
    virtual u32 TypeId() const noexcept = 0;

    Boolean Is(EventCategory cat) const noexcept
    {
        return (Category() & cat) != EventCategory::None;
    }

    template<typename TEnum>
        requires(is_enum_v<TEnum>)
    constexpr TEnum Type() const noexcept
    {
        using U = underlying_type_t<TEnum>;

        static_assert(sizeof(typename decltype(TypeId())::value_type) >= sizeof(U),
            "TypeId wrapper too small for this enum");

        return static_cast<TEnum>(
            static_cast<U>(TypeId())
            );
    }

    template<typename T>
    const T& As() const noexcept
    {
        return static_cast<const T&>(*this);
    }

    TimePoint Timestamp;
    Boolean Handled = false;
};