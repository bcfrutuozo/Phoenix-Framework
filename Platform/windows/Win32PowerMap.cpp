#include "Win32PowerMap.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

SystemEventType TranslatePowerEvent(u64 wp) noexcept
{
    switch (wp)
    {
    case PBT_APMBATTERYLOW:
        return SystemEventType::PowerBatteryLow;
    case PBT_APMSUSPEND:
        return SystemEventType::PowerSuspend;
    case PBT_APMRESUMEAUTOMATIC:
    case PBT_APMRESUMECRITICAL:
    case PBT_APMRESUMESUSPEND:
        return SystemEventType::PowerResume;
    case PBT_APMPOWERSTATUSCHANGE:
        return SystemEventType::PowerUnknown;
    default:
        return SystemEventType::PowerUnknown;
    }
}