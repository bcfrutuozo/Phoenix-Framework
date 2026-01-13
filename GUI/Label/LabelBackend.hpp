#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Core/UIHandle.hpp"

struct ControlBackend;
struct WindowBackend;
class Control;

ControlBackend* CreateLabelBackend(Control* owner, WindowBackend* parent, const String& text, u32 x, u32 y);