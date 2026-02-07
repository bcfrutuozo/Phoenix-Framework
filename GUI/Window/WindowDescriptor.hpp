#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "GUI/Drawing/Font.hpp"
#include "GUI/Core/HorizontalAlignment.hpp"
#include "GUI/Core/VerticalAlignment.hpp"
#include "GUI/Core/TextFormat.hpp"

class UIContext;

struct WindowDescriptor : public Object<WindowDescriptor>
{
    String Title;
    Size Size;
    Boolean Resizable = true;
    Point Location;
    UIContext* UIContext = nullptr;
    Color BackgroundColor = Color::Black;
    Color ForeColor = Color::White;
    Font* Font = nullptr;
    HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
    TextFormat TextFormat = TextFormat::None;
};

using WindowDesc = WindowDescriptor;