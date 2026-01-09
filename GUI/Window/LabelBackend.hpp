#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Window/WindowHandle.hpp"

struct ControlBackend;
struct LabelBackend;
class Control;

ControlBackend* CreateLabelBackend(Control* owner, UIHandle parent, const String& text, u32 x, u32 y);