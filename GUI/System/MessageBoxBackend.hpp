#pragma once

#include "GUI/Core/UIHandle.hpp"
#include "GUI/System/MessageBoxDescriptor.hpp"
#include "System/String.hpp"

struct WindowBackend;
struct MessageBoxDescriptor;

MessageBoxResult CreateMessageBox(WindowBackend* window, const MessageBoxDescriptor* desc);
MessageBoxResult TranslateResult(i32 code);