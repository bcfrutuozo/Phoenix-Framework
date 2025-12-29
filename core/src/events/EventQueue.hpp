#pragma once

#include "types/Boolean.hpp"
#include "Event.hpp"

class EventQueue
{
public:
    static void Push(Event* e) noexcept;
    static Boolean Poll(Event*& out) noexcept;
    static void Clear() noexcept;
};