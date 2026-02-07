#include "Win32PowerMap.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

PowerEventType TranslatePowerEvent(u64 wp) noexcept
{
    switch (wp)
    {
    case PBT_APMBATTERYLOW:
        return PowerEventType::BatteryLow;
    case PBT_APMSUSPEND:
        return PowerEventType::Suspend;
    case PBT_APMRESUMEAUTOMATIC:
    case PBT_APMRESUMECRITICAL:
    case PBT_APMRESUMESUSPEND:
        return PowerEventType::Resume;
    case PBT_APMPOWERSTATUSCHANGE:
        return PowerEventType::Unknown;
    default:
        return PowerEventType::Unknown;
    }
}