#pragma once

#include "Event.hpp"
#include "EventCategory.hpp"

class EventDispatcher
{
public:

    explicit EventDispatcher(const Event& e) noexcept
        : _event(e)
    { }

    template<typename T, typename Enum, typename Fn>
    Boolean Dispatch(EventCategory category, Enum type, Fn&& fn) const noexcept
    {
        if (_event.Category() != category)
            return false;

        if (_event.TypeId() != static_cast<uint32_t>(type))
            return false;

        fn(static_cast<const T&>(_event));
        return true;
    }
    
private:
    const Event& _event;
};