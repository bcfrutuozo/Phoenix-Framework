#include "GUI/Label/LabelBackend.hpp"
#include "GUI/Label/Label.hpp"
#include "Win32ObjectHeader.hpp"
#include "Win32WndProc.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static const wchar_t* kLabelClassName = L"Phoenix_Label";

// ===============================
// Label class registration
// ===============================
static void RegisterLabelClass()
{
	static bool registered = false;
	if (registered)
		return;

	WNDCLASSW wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Win32LabelProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = kLabelClassName;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);
	registered = true;
}

ControlBackend* CreateLabelBackend(Control* control, WindowBackend* parent)
{
	RegisterLabelClass();

	HWND parentHwnd = parent->hwnd;

	auto* backend = new LabelBackend{};
	backend->owner = control;
	backend->queue = parent->queue;
	backend->context = parent->context;

	backend->hwnd = CreateWindowExW(
		0,
		kLabelClassName,
		nullptr, // Text is set OnPaint()
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		backend->owner->GetLocation().GetX(),
		backend->owner->GetLocation().GetY(),
		backend->owner->GetSize().GetWidth(),
		backend->owner->GetSize().GetHeight(),
		parentHwnd,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	return backend;
}