#include "GUI/Core/ControlBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "System/Events/EventQueue.hpp"

void DestroyControlBackend(ControlBackend* backend)
{
	DestroyWindow(backend->hwnd);
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

UIHandle GetControlBackendHandle(ControlBackend* backend)
{
	UIHandle h{};
	h.Handle = Pointer(backend->hwnd);
	h.Type = UIHandle::Type::Control;
	return h;
}