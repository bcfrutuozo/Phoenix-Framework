#include "GUI/Core/ControlBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void DestroyControlBackend(ControlBackend* backend)
{
	DestroyWindow(backend->hwnd);
	delete backend;
}

void ShowControlBackend(ControlBackend* backend)
{
	ShowWindow(backend->hwnd, SW_SHOW);
}

void UpdateControlBackend(ControlBackend* backend)
{
	UpdateWindow(backend->hwnd);
}

void SetEnabledControlBackend(ControlBackend* backend, Boolean enabled)
{
	EnableWindow(backend->hwnd, enabled);
}

void AttachEventQueueToControlBackend(ControlBackend* backend, EventQueue* queue)
{
	backend->queue = queue;
}

UIHandle GetControlNativeHandle(ControlBackend* backend)
{
	UIHandle h;
	h.Handle = Pointer(backend->hwnd);
	h.Type = UIHandle::Type::Control;
	return h;
}