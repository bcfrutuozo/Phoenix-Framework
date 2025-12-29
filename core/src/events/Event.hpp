#pragma once

#include "EventType.hpp"
#include "time/TimePoint.hpp"

struct Event
{
    EventType type = EventType::Custom;
    TimePoint timestamp;
};