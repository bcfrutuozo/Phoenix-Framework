#include "GUI/Label/LabelBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "Win32WndProc.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

ControlBackend* CreateLabelBackend(Control* control, WindowBackend* parent, const String& text, u32 x, u32 y)
{
	HWND parentHwnd = parent->hwnd;

	auto* backend = new LabelBackend{};
	backend->owner = control;
	backend->queue = parent->queue;

	backend->hwnd = CreateWindowExW(
		0,
		L"STATIC",
		text.ToWideCharArray().GetData(),
		WS_CHILD | WS_VISIBLE | SS_NOTIFY,
		x,
		y,
		100, 100,
		parentHwnd,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	// 🔑 ESSENCIAL
	SetWindowLongPtr(
		backend->hwnd,
		GWLP_USERDATA,
		reinterpret_cast<LONG_PTR>(backend)
	);

	backend->Kind = Win32ObjectKind::Control;

	// Fonte (ESSENCIAL)
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessageW(backend->hwnd, WM_SETFONT, (WPARAM)font, TRUE);

	// Subclass
	SetWindowSubclass(
		backend->hwnd,
		Phoenix_Win32SubclassProc,
		0,
		reinterpret_cast<DWORD_PTR>(backend)
	);

	return backend;
}