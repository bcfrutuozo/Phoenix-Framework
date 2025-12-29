#pragma once

#include "meta/TypeTraits.hpp"
#include "EventCategory.hpp"
#include "types/Boolean.hpp"
#include "types/UInt32.hpp"
#include "time/TimePoint.hpp"

class Event
{
public:

    virtual ~Event() = default;

    virtual EventCategory Category() const noexcept = 0;
    virtual UInt32 TypeId() const noexcept = 0;

    Boolean Is(EventCategory cat) const noexcept
    {
        return (Category() & cat) != EventCategory::None;
    }

    template<typename TEnum>
    TEnum Type() const noexcept
    {
        static_assert(is_enum<TEnum>::value, "TEnum must be an enum");
        return static_cast<TEnum>(static_cast<uint32_t>(TypeId()));
    }

    template<typename T>
    const T& As() const noexcept
    {
        return static_cast<const T&>(*this);
    }

    TimePoint timestamp;
};