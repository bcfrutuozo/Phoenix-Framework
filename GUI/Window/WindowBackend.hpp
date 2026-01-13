#pragma once

#include "GUI/Core/UIHandle.hpp"
#include "GUI/Window/WindowDescriptor.hpp"
#include "System/String.hpp"

struct WindowBackend;
class EventQueue;
class Window;

EventQueue* GetEventQueue(WindowBackend*);

WindowBackend* CreateWindowBackend(Window*, const WindowDesc* desc);

void AttachEventQueue(WindowBackend*, EventQueue*);
void DestroyWindowBackend(WindowBackend*);
void ShowWindowBackend(WindowBackend*);
void UpdateWindowBackend(WindowBackend* backend);
void PollWindowBackendEvents(WindowBackend*);

SurfaceHandle GetWindowSurfaceHandle(WindowBackend*);

String GetWindowTitle(WindowBackend*);
void SetWindowTitle(WindowBackend*, const WindowDesc*);
void SetWindowSize(WindowBackend*, const WindowDesc*);
void SetWindowLocation(WindowBackend*, const WindowDesc*);
void SetWindowResize(WindowBackend*, const WindowDesc*);