#pragma once

// Cross-platform API
// Converts CPU divide-by-zero into C++ exceptions.

int  EnableDivideByZeroTrapGlobal(int enableFloatTraps);
void DisableDivideByZeroTrapGlobal();

template<typename Callable>
void RunWithDivideByZeroGuard(Callable&& fn);

template<typename Callable>
int TryRunCatchDivideByZero(Callable&& fn);