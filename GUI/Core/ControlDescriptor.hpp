#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Drawing/Font.hpp"
#include "System/Types/Drawing/Color.hpp"
#include "System/Types/Drawing/Point.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "HorizontalAlignment.hpp"
#include "VerticalAlignment.hpp"
#include "TextFormat.hpp"

class UIContext;
class Window;

struct ControlDescriptor : public Object<ControlDescriptor>
{
    Boolean AutoSize = true;
    Size Size;
    Color BackgroundColor = Color::White;
    String Text;
    Color ForeColor = Color::Black;
    Font* Font = nullptr;
    Point Location;
    HorizontalAlignment HorizontalAlignment;
    VerticalAlignment VerticalAlignment;
    TextFormat TextFormat;
    Window* Parent;
    UIContext* UIContext;
};