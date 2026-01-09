#pragma once

#include "System/Events/EventCategory.hpp"

#include <cstdint>

PowerEventType TranslatePowerEvent(uint32_t wp) noexcept;