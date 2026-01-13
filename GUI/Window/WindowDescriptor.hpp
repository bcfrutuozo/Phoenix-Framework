#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Point.hpp"
#include "GUI/Drawing/Size.hpp"

struct WindowDescriptor : public Object<WindowDescriptor>
{
    String Title;
    Size Size;
    Boolean Resizable = true;
    Point Location;
};

using WindowDesc = WindowDescriptor;