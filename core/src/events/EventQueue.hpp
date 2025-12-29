#pragma once

#include "types/Boolean.hpp"
#include "Event.hpp"

class EventQueue
{
public:
    static void Push(const Event& e) noexcept;
    static Boolean Poll(Event& out) noexcept;
    static void Clear() noexcept;
};