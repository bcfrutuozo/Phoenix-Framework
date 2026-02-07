#pragma once

// ============================================
//  Phoenix Framework Interface
// ============================================

#include "Globals.hpp"
#include "DivideByZeroTrap.hpp"
#include "Exceptions.hpp"
#include "Types.hpp"
#include "CPUInfo.hpp"
#include "Console/Console.hpp"
#include "System/Time/Clock.hpp"

// ---------------------------------------------------------
// Public API
// ---------------------------------------------------------

void InitFramework();
void CloseFramework();
uint64_t GenerateRandomSeed() noexcept;