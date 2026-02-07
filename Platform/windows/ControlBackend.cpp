#include "GUI/Core/ControlBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Core/Control.hpp"
#include "GUI/Context/FontManager.hpp"
#include "Win32Helpers.hpp"

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

Size CalculateControlSizeByText(Control* owner, FontManager* fontManager)
{
	RECT rc{ 0,0,0,0 };

	HDC hdc = GetDC(nullptr);

	HFONT hfont = (HFONT)fontManager->GetNativeObject(owner->GetFont()).Get();
	HGDIOBJ old = SelectObject(hdc, hfont);
	Array<wchar_t> text = owner->GetText().ToWideCharArray();

	UINT drawFlags = Win32ConvertTextFlags(owner->GetHorizontalAlignment(), owner->GetVerticalAlignment(), owner->GetTextFormat());

	DrawTextExW(
		hdc,
		text.GetData(),
		text.GetLength(),
		&rc,
		drawFlags | DT_CALCRECT,
		nullptr
	);

	DeleteDC(hdc);
	DeleteObject(old);

	return Size((uint32_t)(rc.right - rc.left), (uint32_t)(rc.bottom - rc.top));
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