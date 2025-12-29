#include "window/WindowBackend.hpp"
#include "events/WindowEvents.hpp"
#include "events/MouseEvents.hpp"
#include "events/KeyboardEvents.hpp"
#include "events/TextEvents.hpp"
#include "events/SystemEvents.hpp"
#include "Win32KeyMap.hpp"
#include "Win32PowerMap.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

#include <imm.h>
#pragma comment(lib, "imm32.lib")

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

	WindowHandle handle = backend
		? GetWindowBackendHandle(backend)
		: WindowHandle{};

	switch (msg)
	{
		// -------------------- Window --------------------

	case WM_SIZE:
		backend->queue->Push(new WindowResizeEvent(handle, LOWORD(lp), HIWORD(lp)));
		return 0;

	case WM_MOVE:
		backend->queue->Push(new WindowMoveEvent(handle, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp)));
		return 0;

	case WM_SETFOCUS:
		backend->queue->Push(new WindowFocusGainedEvent(handle));
		return 0;

	case WM_KILLFOCUS:
		backend->queue->Push(new WindowFocusLostEvent(handle));
		return 0;

	case WM_SHOWWINDOW:
		if (wp)
			backend->queue->Push(new WindowShowEvent(handle));
		else
			backend->queue->Push(new WindowHideEvent(handle));
		return 0;

	case WM_DPICHANGED:
		backend->queue->Push(new WindowDPIChangedEvent(handle, HIWORD(wp)));
		return 0;

		// -------------------- Mouse --------------------

	case WM_MOUSEMOVE:
		backend->queue->Push(new MouseMoveEvent(handle, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
		return 0;

	case WM_LBUTTONDOWN:
		backend->queue->Push(new MouseButtonDownEvent(handle, MouseButton::Left));
		return 0;

	case WM_LBUTTONUP:
		backend->queue->Push(new MouseButtonUpEvent(handle, MouseButton::Left));
		return 0;

	case WM_RBUTTONDOWN:
		backend->queue->Push(new MouseButtonDownEvent(handle, MouseButton::Right));
		return 0;

	case WM_RBUTTONUP:
		backend->queue->Push(new MouseButtonUpEvent(handle, MouseButton::Right));
		return 0;

	case WM_MOUSEWHEEL:
		backend->queue->Push(new MouseScrollEvent(handle, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA));
		return 0;

	case WM_MOUSEHWHEEL:
		backend->queue->Push(new MouseScrollEvent(handle, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA, 0.0f));
		return 0;

		// -------------------- Keyboard --------------------

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		backend->queue->Push(new KeyDownEvent(handle, TranslateKey((uint32_t)wp, (uint32_t)lp), (lp & (1 << 30)) != 0));
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		backend->queue->Push(new KeyUpEvent(handle, TranslateKey((uint32_t)wp, (uint32_t)lp)));
		return 0;

		// -------------------- Text / IME --------------------

	case WM_CHAR:
		backend->queue->Push(new TextInputEvent(handle, static_cast<uint32_t>(wp)));
		return 0;

		// IME reservado (implementar depois)
	case WM_IME_STARTCOMPOSITION:
	{
		backend->queue->Push(new ImeCompositionEvent(
			handle,
			ImeCompositionType::Start,
			String::Empty(),
			0
		));
		return 0;
	}
	case WM_IME_COMPOSITION:
	{
		HIMC imc = ImmGetContext(hwnd);
		if (!imc)
			break;

		// ---------------- Update (texto em composição) ----------------
		if (lp & GCS_COMPSTR)
		{
			LONG bytes = ImmGetCompositionStringW(
				imc,
				GCS_COMPSTR,
				nullptr,
				0
			);

			if (bytes > 0)
			{
				const uint32_t len = bytes / sizeof(wchar_t);

				List<wchar_t> buffer;
				buffer.Resize(len + 1);

				ImmGetCompositionStringW(
					imc,
					GCS_COMPSTR,
					buffer.Data(),
					bytes
				);

				buffer[len] = L'\0';

				String text(buffer.Data(), len);

				LONG cursor = ImmGetCompositionStringW(
					imc,
					GCS_CURSORPOS,
					nullptr,
					0
				);

				backend->queue->Push(new ImeCompositionEvent(
					handle,
					ImeCompositionType::Update,
					text,
					static_cast<UInt32>((uint32_t)cursor)
				));
			}
		}

		// ---------------- Commit (texto final) ----------------
		if (lp & GCS_RESULTSTR)
		{
			LONG bytes = ImmGetCompositionStringW(
				imc,
				GCS_RESULTSTR,
				nullptr,
				0
			);

			if (bytes > 0)
			{
				const uint32_t len = bytes / sizeof(wchar_t);

				List<wchar_t> buffer;
				buffer.Resize(len + 1);

				ImmGetCompositionStringW(
					imc,
					GCS_RESULTSTR,
					buffer.Data(),
					bytes
				);

				buffer[len] = L'\0';

				String text(buffer.Data(), len);

				backend->queue->Push(new ImeCompositionEvent(
					handle,
					ImeCompositionType::Commit,
					text,
					text.GetByteCount() // ou ByteCount, conforme sua API
				));
			}
		}

		ImmReleaseContext(hwnd, imc);
		return 0;
	}
	case WM_IME_ENDCOMPOSITION:
	{
		backend->queue->Push(new ImeCompositionEvent(
			handle,
			ImeCompositionType::End,
			String::Empty(),
			0
		));
		return 0;
	}
		// -------------------- System --------------------

	case WM_QUIT:
		backend->queue->Push(new SystemQuitEvent());
		return 0;

	case WM_ENDSESSION:
		backend->queue->Push(new SystemShutdownEvent());
		return 0;

	case WM_POWERBROADCAST:
		switch (TranslatePowerEvent(wp))
		{
			case PowerEventType::BatteryLow: backend->queue->Push(new SystemBatterLowPowerEvent());
				break;
			case PowerEventType::Resume: backend->queue->Push(new SystemResumePowerEvent());
				break;
			case PowerEventType::Suspend: backend->queue->Push(new SystemSuspendPowerEvent());
				break;
			case PowerEventType::Unknown: backend->queue->Push(new SystemUnknownPowerEvent());
				break;
		}
		
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

void PollWindowBackendEvents(WindowBackend* backend)
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}