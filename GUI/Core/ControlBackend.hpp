#pragma once

#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"

struct ControlBackend;
class EventQueue;

void DestroyControlBackend(ControlBackend* backend);
void ShowControlBackend(ControlBackend* backend);
void UpdateControlBackend(ControlBackend* backend);
void SetEnabledControlBackend(ControlBackend* backend, Boolean enabled);
void AttachEventQueueToControlBackend(ControlBackend* backend, EventQueue* queue);
UIHandle GetControlNativeHandle(ControlBackend*);
String GetControlParent(ControlBackend*);