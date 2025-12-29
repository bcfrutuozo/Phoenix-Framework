#include "window/WindowBackend.hpp"
#include "events/WindowEvents.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// ===============================
// Backend struct (privado)
// ===============================
struct WindowBackend
{
	HWND hwnd;
	EventQueue* queue;
};

static const wchar_t* kWindowClassName = L"Phoenix_Window";

// ===============================
// Win32 WndProc (TOTALMENTE LOCAL)
// ===============================
static LRESULT CALLBACK Win32WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wp,
	LPARAM lp)
{
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* backend = static_cast<WindowBackend*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(backend));

		backend->hwnd = hwnd;
	}

	auto* backend = reinterpret_cast<WindowBackend*>(
		GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (!backend)
		return DefWindowProc(hwnd, msg, wp, lp);

	switch (msg)
	{
	case WM_CLOSE:
		backend->queue->Push(WindowCloseEvent{
			GetWindowBackendHandle(backend)
			});
		return 0;

	case WM_SIZE:
		backend->queue->Push(WindowResizeEvent{
			GetWindowBackendHandle(backend),
			LOWORD(lp),
			HIWORD(lp)
			});
		return 0;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

// ===============================
// Window class registration
// ===============================
static void RegisterWindowClass()
{
	static bool registered = false;
	if (registered)
		return;

	WNDCLASSW wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Win32WndProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = kWindowClassName;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);
	registered = true;
}

WindowHandle GetWindowBackendHandle(WindowBackend* backend)
{
	WindowHandle h{};
	h.window.value = Pointer(backend->hwnd);
	h.display.value = Pointer(GetModuleHandle(nullptr));
	return h;
}

// ===============================
// Backend API (usada por Window.cpp)
// ===============================
WindowBackend* CreateWindowBackend(const WindowDesc& desc, EventQueue& queue)
{
	RegisterWindowClass();

	auto* backend = new WindowBackend{};
	backend->queue = &queue;

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

	return backend;
}

void DestroyWindowBackend(WindowBackend* backend)
{
	if (!backend)
		return;

	DestroyWindow(backend->hwnd);
	delete backend;
}

void ShowWindowBackend(WindowBackend* backend)
{
	ShowWindow(backend->hwnd, SW_SHOW);
	UpdateWindow(backend->hwnd);
}

void PollWindowBackendEvents(WindowBackend*)
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}