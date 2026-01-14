#pragma once

#include "Event.hpp"
#include "EventCategory.hpp"

class EventDispatcher
{
public:

    explicit EventDispatcher(Event& e) noexcept
        : _event(e)
    { }

    template<typename T, typename Enum, typename Fn>
    Boolean Dispatch(EventCategory category, Enum type, Fn&& fn) const noexcept
    {
        if (_event.Category() != category)
            return false;

        if (_event.TypeId() != FromEnum<u32>(type))
            return false;

        if (_event.Handled)
            return false;

        fn(static_cast<T&>(_event));
        return true;
    }
    
private:
    Event& _event;
};