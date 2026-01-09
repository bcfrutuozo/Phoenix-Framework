#pragma once

#include "WindowHandle.hpp"
#include "GUI/Window/WindowDescriptor.hpp"
#include "System/Events/EventQueue.hpp"

struct WindowBackend;

WindowBackend* CreateWindowBackend(Window*, const WindowDesc& desc);

void AttachEventQueue(WindowBackend*, EventQueue*);
void DestroyWindowBackend(WindowBackend*);
void ShowWindowBackend(WindowBackend*);
void UpdateWindowBackend(WindowBackend* backend);
void PollWindowBackendEvents(WindowBackend*);

UIHandle GetWindowBackendHandle(WindowBackend*);
SurfaceHandle GetWindowSurfaceHandle(WindowBackend*);