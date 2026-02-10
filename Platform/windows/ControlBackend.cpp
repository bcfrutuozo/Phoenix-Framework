#include "GUI/Core/ControlBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Core/Control.hpp"
#include "GUI/Context/FontManager.hpp"
#include "GUI/Context/UIContext.hpp"
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

Size CalculateControlSizeByText(ControlBackend* backend)
{
	Control* c = backend->owner;
	FontManager* fm = backend->context->GetFontManager();

	RECT rc{ 0,0, c->GetWidth(), c->GetHeight()};

	HDC hdc = GetDC(nullptr);

	HFONT hfont = (HFONT)fm->GetNativeObject(c->GetFont()).Get();
	HGDIOBJ old = SelectObject(hdc, hfont);
	Array<wchar_t> text = c->GetText().ToWideCharArray();

	UINT drawFlags = Win32ConvertTextFlags(c->GetHorizontalAlignment(), c->GetVerticalAlignment(), c->GetTextFormat());

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
	InvalidateRect(backend->hwnd, NULL, TRUE);
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

void ResizeControl(ControlBackend* backend)
{
	Control* c = backend->owner;
	SetWindowPos(backend->hwnd, 
		nullptr, 
		0, 
		0, 
		c->GetWidth(), 
		c->GetHeight(), 
		SWP_NOMOVE);
}