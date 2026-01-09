#pragma once

#include "System/Types.hpp"
#include "GUI/Window/WindowHandle.hpp"

struct ControlBackend;
class EventQueue;

void DestroyControlBackend(ControlBackend* backend);
void ShowControlBackend(ControlBackend* backend);
void UpdateControlBackend(ControlBackend* backend);
void SetEnabledControlBackend(ControlBackend* backend, Boolean enabled);
void AttachEventQueueToControlBackend(ControlBackend* backend, EventQueue* queue);

UIHandle GetControlBackendHandle(ControlBackend*);