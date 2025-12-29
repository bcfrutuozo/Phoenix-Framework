#pragma once

#include "WindowHandle.hpp"
#include "window/WindowDescriptor.hpp"
#include "events/EventQueue.hpp"

struct WindowBackend;

WindowBackend* CreateWindowBackend(const WindowDesc& desc, EventQueue& queue);

void DestroyWindowBackend(WindowBackend*);
void ShowWindowBackend(WindowBackend*);
void PollWindowBackendEvents(WindowBackend*);

WindowHandle GetWindowBackendHandle(WindowBackend*);