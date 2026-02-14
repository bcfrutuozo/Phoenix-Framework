#pragma once

#include "System/Types.hpp"
#include "Event.hpp"
#include "EventCategory.hpp"

class EventDispatcher
{
public:

    explicit EventDispatcher(Event& e) noexcept
        : _event(e)
    { }

    template<typename T, typename u8, typename Fn>
    Boolean Dispatch(EventCategory category, u8 type, Fn&& fn) const noexcept
    {
        if (_event.GetCategory() != category)
            return false;

        if (_event.GetType() != type)
            return false;

        if (_event.Has(EventFlags::Handled))
            return false;

        if (!_event.Has(EventFlags::Propagable)) {
            EventConsumeGuard guard(_event); // Force consume event in case of throw
            fn(static_cast<T&>(_event));
        }
        else {
            fn(static_cast<T&>(_event));
        }

        return true;
    }
    
private:

    struct EventConsumeGuard
    {
        Event& e;
        bool active = true;

        explicit EventConsumeGuard(Event& ev) noexcept
            : e(ev) {
        }

        ~EventConsumeGuard() noexcept
        {
            if (active)
                e.Set(EventFlags::Consumed);
        }

        void Dismiss() noexcept
        {
            active = false;
        }
    };

    Event& _event;
};