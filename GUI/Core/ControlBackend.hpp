#pragma once

#include "System/Types.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "System/Types/Drawing/Size.hpp"

struct ControlBackend;
class FontManager;
class EventQueue;

void DestroyControlBackend(ControlBackend* backend);
void ShowControlBackend(ControlBackend* backend);
void UpdateControlBackend(ControlBackend* backend);
void SetEnabledControlBackend(ControlBackend* backend, Boolean enabled);
void AttachEventQueueToControlBackend(ControlBackend* backend, EventQueue* queue);
UIHandle GetControlNativeHandle(ControlBackend*);
String GetControlParent(ControlBackend*);
Size CalculateControlSizeByText(ControlBackend*);

void ResizeControl(ControlBackend*);