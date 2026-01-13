#pragma once

// ============================================
//  Phoenix Framework Interface
// ============================================

#include "Globals.hpp"
#include "DivideByZeroTrap.hpp"
#include "Exceptions.hpp"
#include "Types.hpp"
#include "Console/Console.hpp"

// ---------------------------------------------------------
// Public API
// ---------------------------------------------------------

void InitFramework();
uint64_t GenerateRandomSeed() noexcept;