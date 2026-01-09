#pragma once

#include "Application.hpp"
#include "ConsoleApplication.hpp"
#include "GUIApplication.hpp"

enum class AppKind
{
    Console,
    Windowed
};

inline ConsoleApplication* CreateConsoleApplication(AppKind kind)
{
    return new ConsoleApplication();
}

inline GUIApplication* CreateGUIApplication(AppKind kind)
{
    return new GUIApplication();
}