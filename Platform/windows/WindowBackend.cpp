#include "GUI/Window/WindowBackend.hpp"
#include "GUI/events/UIEvents.hpp"
#include "GUI/Events/MouseEvents.hpp"
#include "GUI/Events/KeyboardEvents.hpp"
#include "GUI/Events/TextEvents.hpp"
#include "System/Events/SystemEvents.hpp"
#include "System/String.hpp"
#include "System/Collections/List.hpp"
#include "Win32ObjectHeader.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "Win32WndProc.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

static const wchar_t* kWindowClassName = L"Phoenix_Window";

// ===============================
// Window class registration
// ===============================
static void RegisterWindowClass()
{
	static bool registered = false;
	if (registered)
		return;

	// For subclasses comctl32 initialization
	INITCOMMONCONTROLSEX icc{};
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icc);

	WNDCLASSW wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Win32WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = kWindowClassName;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);
	registered = true;
}

UIHandle GetWindowBackendHandle(WindowBackend* backend)
{
	UIHandle h{};
	h.Handle = Pointer(backend->hwnd);
	h.Type = UIHandle::Type::Window;
	return h;
}

SurfaceHandle GetWindowSurfaceHandle(WindowBackend* backend)
{
	SurfaceHandle h{ Pointer(backend->hwnd), Pointer(GetModuleHandle(nullptr)) };
	return h;
}

// ===============================
// Backend API (usada por Window.cpp)
// ===============================
WindowBackend* CreateWindowBackend(Window* window, const WindowDesc& desc)
{
	RegisterWindowClass();

	auto* backend = new WindowBackend{};

	DWORD style = WS_OVERLAPPEDWINDOW;
	if (!desc.resizable)
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	backend->hwnd = CreateWindowExW(
		0,
		kWindowClassName,
		desc.title.ToWideCharArray().begin(),
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		desc.width,
		desc.height,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	backend->owner = window;

	return backend;
}

void AttachEventQueue(WindowBackend* backend, EventQueue* queue)
{
	backend->queue = queue;
}

void DestroyWindowBackend(WindowBackend* backend)
{
	if (!backend)
		return;

	DestroyWindow(backend->hwnd);
}

void ShowWindowBackend(WindowBackend* backend)
{
	ShowWindow(backend->hwnd, SW_SHOW);
}

void UpdateWindowBackend(WindowBackend* backend)
{
	UpdateWindow(backend->hwnd);
}

void PollWindowBackendEvents(WindowBackend* backend)
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}