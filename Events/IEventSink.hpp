#pragma once

#include "System/Types.hpp"

class Event;

class IEventSink : public Object<IEventSink>
{

public:
    virtual ~IEventSink() = default;
    virtual void Dispatch(Event& e) = 0;
};