#pragma once

#include "Application.hpp"
#include "ConsoleApplication.hpp"
#include "GUIApplication.hpp"

inline ConsoleApplication* CreateConsoleApplication()
{
    return new ConsoleApplication();
}

inline GUIApplication* CreateGUIApplication()
{
    return new GUIApplication();
}