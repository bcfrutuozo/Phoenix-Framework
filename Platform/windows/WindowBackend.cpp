#include "GUI/Window/WindowBackend.hpp"
#include "Events/EventQueue.hpp"
#include "System/String.hpp"
#include "Win32ObjectHeader.hpp"
#include "Win32WndProc.hpp"
#include "GUI/Window/Window.hpp"
#include "System/Exceptions.hpp"

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
	wc.lpfnWndProc = Phoenix_Win32WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = kWindowClassName;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);
	registered = true;
}

SurfaceHandle GetWindowSurfaceHandle(WindowBackend* backend)
{
	SurfaceHandle h{ Pointer(backend->hwnd), Pointer(GetModuleHandle(nullptr)) };
	return h;
}

EventQueue* GetEventQueue(WindowBackend* backend)
{
	return backend->queue;
}

// ===============================
// Backend API (usada por Window.cpp)
// ===============================
WindowBackend* CreateWindowBackend(Window* window, const WindowDesc* desc)
{
	RegisterWindowClass();

	auto* backend = new WindowBackend{};

	DWORD style = WS_OVERLAPPEDWINDOW;
	if (!desc->Resizable)
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_EX_APPWINDOW);

	backend->hwnd = CreateWindowExW(
		0,
		kWindowClassName,
		desc->Title.ToWideCharArray().GetData(),
		style,
		desc->Location.GetX(),
		desc->Location.GetY(),
		desc->Size.GetWidth(),
		desc->Size.GetHeight(),
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

	auto ret = DestroyWindow(backend->hwnd);
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void ShowWindowBackend(WindowBackend* backend)
{
	ShowWindow(backend->hwnd, SW_SHOW);
}

void UpdateWindowBackend(WindowBackend* backend)
{
	auto ret = UpdateWindow(backend->hwnd);
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
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

String GetWindowTitle(WindowBackend* backend)
{
	wchar_t buf[256];
	int n = GetWindowTextW(backend->hwnd, buf, 256);
	return String(buf, n);
}

void SetWindowTitle(WindowBackend* backend, const WindowDesc* desc)
{
	auto ret = SetWindowTextW(backend->hwnd, desc->Title.ToWideCharArray().GetData());
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowSize(WindowBackend* backend, const WindowDesc* desc)
{
	auto ret = SetWindowPos(
		backend->hwnd,
		nullptr,   // não altera z-order
		0,
		0,
		desc->Size.GetWidth(),
		desc->Size.GetHeight(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
	);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowLocation(WindowBackend* backend, const WindowDesc* desc)
{
	auto ret = SetWindowPos(
		backend->hwnd,
		nullptr,   // não altera z-order
		desc->Location.GetX(),
		desc->Location.GetY(),
		0,
		0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
	);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowResize(WindowBackend* backend, const WindowDesc* desc)
{
	LONG_PTR style = GetWindowLongPtr(backend->hwnd, GWL_STYLE);

	if (desc->Resizable)
		style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
	else
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	SetWindowLongPtr(backend->hwnd, GWL_STYLE, style);

	auto ret = SetWindowPos(backend->hwnd, nullptr, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}