#pragma once

#include "System/Types.hpp"
#include "System/String.hpp"
#include "GUI/Core/UIHandle.hpp"
#include "System/Types/Drawing/Size.hpp"
#include "GUI/System/MessageBoxDescriptor.hpp"

struct NativeBackend;
class EventQueue;
class UIContext;
class IEventSink;

class Window;
class Label;
class TextBox;

// Globals
void PollBackendEvents();

// Controls
void ShowControlBackend(NativeBackend* backend);
void UpdateControlBackend(NativeBackend* backend);
void SetEnabledControlBackend(NativeBackend* backend, Boolean enabled);
Size CalculateControlSizeByText(NativeBackend* backend);
void ResizeControl(NativeBackend* backend);
void DestroyWindowBackend(NativeBackend* backend);
void ShowWindowBackend(NativeBackend* backend);
void HideControlBackend(NativeBackend* backend);
void UpdateWindowBackend(NativeBackend* backend);
void NB_Enable(NativeBackend* backend);
void NB_Disable(NativeBackend* backend);
SurfaceHandle GetWindowSurfaceHandle(NativeBackend* backend);
String GetWindowTitle(NativeBackend* backend);
void SetWindowTitle(NativeBackend* backend);
void SetWindowSize(NativeBackend* backend);
void SetWindowLocation(NativeBackend* backend);
void SetWindowResize(NativeBackend* backend);

// Create
NativeBackend* CreateWindowBackend(Window* window, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink);
NativeBackend* CreateLabelBackend(Label* label, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink);
NativeBackend* CreateTextBoxBackend(TextBox* textBox, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink);

// Destroy
void DestroyControlBackend(NativeBackend* backend);
void DestroyTextBoxBackend(NativeBackend* backend);

// MessageBox
MessageBoxResult CreateMessageBox(NativeBackend* backend, MessageBoxDescriptor* desc);
MessageBoxResult TranslateResult(i32 code);