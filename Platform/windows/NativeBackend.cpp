#include "GUI/Core/NativeBackend.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Controls.hpp"
#include "GUI/Context/FontManager.hpp"
#include "GUI/Context/UIContext.hpp"
#include "Win32Helpers.hpp"
#include "Win32WndProc.hpp"
#include "GUI/System/MessageBoxDescriptor.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

void DestroyControlBackend(NativeBackend* backend)
{
	DestroyWindow(backend->Handle);
	delete backend;
}

void ShowControlBackend(NativeBackend* backend)
{
	ShowWindow(backend->Handle, SW_SHOW);
}

void HideControlBackend(NativeBackend* backend)
{
	ShowWindow(backend->Handle, SW_HIDE);
}

void NB_Enable(NativeBackend* backend)
{
	EnableWindow(backend->Handle, TRUE);
}

void NB_Disable(NativeBackend* backend)
{
	EnableWindow(backend->Handle, FALSE);
}

Size CalculateControlSizeByText(NativeBackend* backend)
{
	ControlBase* c = backend->Owner;
	FontManager* fm = backend->Context->GetFontManager();

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

void UpdateControlBackend(NativeBackend* backend)
{
	InvalidateRect(backend->Handle, NULL, TRUE);
}

void SetEnabledControlBackend(NativeBackend* backend, Boolean enabled)
{
	EnableWindow(backend->Handle, enabled);
}

void ResizeControl(NativeBackend* backend)
{
	ControlBase* c = backend->Owner;
	SetWindowPos(backend->Handle, 
		nullptr, 
		0, 
		0, 
		c->GetWidth(), 
		c->GetHeight(), 
		SWP_NOMOVE);
}


static const wchar_t* kWindowClassName = L"Phoenix_Window";

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
	wc.lpfnWndProc = Win32WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = kWindowClassName;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClassW(&wc);
	registered = true;
}

SurfaceHandle GetWindowSurfaceHandle(NativeBackend* backend)
{
	SurfaceHandle h{ Pointer(backend->Handle), Pointer(GetModuleHandle(nullptr)) };
	return h;
}

// ===============================
// Backend API (usada por Window.cpp)
// ===============================
NativeBackend* CreateWindowBackend(Window* window, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink)
{
	RegisterWindowClass();

	auto* backend = new NativeBackend{};
	backend->Owner = (ControlBase*)window;

	DWORD style = WS_OVERLAPPEDWINDOW;
	if (!backend->Owner->IsResizable())
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX | WS_EX_APPWINDOW);

	if (window->IsVisible())
		style |= WS_VISIBLE;

	if (!window->IsEnabled())
		style |= WS_DISABLED;

	HWND parentHwnd = parentBackend ? parentBackend->Handle : nullptr;

	backend->Handle = CreateWindowExW(
		0,
		kWindowClassName,
		window->GetText().ToWideCharArray().GetData(),
		style,
		window->GetX(),
		window->GetY(),
		window->GetWidth(),
		window->GetHeight(),
		parentHwnd,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	if (!backend->Handle)
		throw Win32Exception(GetLastError());

	backend->Context = uiContext;
	backend->EventQueue = queue;
	backend->EventSink = sink;

	return backend;
}

void DestroyWindowBackend(NativeBackend* backend)
{
	if (!backend)
		return;

	auto ret = DestroyWindow(backend->Handle);
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void ShowWindowBackend(NativeBackend* backend)
{
	ShowWindow(backend->Handle, SW_SHOW);
}

void UpdateWindowBackend(NativeBackend* backend)
{
	auto ret = UpdateWindow(backend->Handle);
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void PollBackendEvents()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

String GetWindowTitle(NativeBackend* backend)
{
	wchar_t buf[256];
	int n = GetWindowTextW(backend->Handle, buf, 256);
	return String(buf, n);
}

void SetWindowTitle(NativeBackend* backend)
{
	auto ret = SetWindowTextW(backend->Handle, backend->Owner->GetText().ToWideCharArray().GetData());
	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowSize(NativeBackend* backend)
{
	auto ret = SetWindowPos(
		backend->Handle,
		nullptr,   // não altera z-order
		0,
		0,
		backend->Owner->GetWidth(),
		backend->Owner->GetHeight(),
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE
	);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowLocation(NativeBackend* backend)
{
	auto ret = SetWindowPos(
		backend->Handle,
		nullptr,   // não altera z-order
		backend->Owner->GetX(),
		backend->Owner->GetY(),
		0,
		0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
	);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

void SetWindowResize(NativeBackend* backend)
{
	LONG_PTR style = GetWindowLongPtr(backend->Handle, GWL_STYLE);

	if (backend->Owner->IsResizable())
		style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
	else
		style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	SetWindowLongPtr(backend->Handle, GWL_STYLE, style);

	auto ret = SetWindowPos(backend->Handle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if (ret == 0)
	{
		throw Win32Exception((uint32_t)GetLastError());
	}
}

static void RegisterTextBoxClass()
{
	static bool registered = false;
	if (registered)
		return;

	INITCOMMONCONTROLSEX icc{};
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icc);

	registered = true;
}

constexpr UINT_PTR TEXTBOX_SUBCLASS_ID = 1;

NativeBackend* CreateTextBoxBackend(TextBox* textBox, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink)
{
	RegisterTextBoxClass();

	auto* backend = new NativeBackend{};
	backend->Owner = textBox;

	HWND parentHwnd = parentBackend ? parentBackend->Handle : nullptr;

	DWORD style = WS_CHILD | WS_CLIPSIBLINGS;

	if (textBox->IsVisible())
		style |= WS_VISIBLE;

	if (!textBox->IsEnabled())
		style |= WS_DISABLED;

	backend->Handle = CreateWindowExW(
		0,
		L"Edit",
		nullptr, // Text is set OnPaint()
		style,
		textBox->GetX(),
		textBox->GetY(),
		textBox->GetWidth(),
		textBox->GetHeight(),
		parentHwnd,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	if (!backend->Handle)
		throw Win32Exception(GetLastError());

	int err = SetWindowSubclass(
		backend->Handle,
		Win32TextBoxProc,
		TEXTBOX_SUBCLASS_ID,
		reinterpret_cast<DWORD_PTR>(backend) // this = TextBox*
	);

	if (!err)
		throw Win32Exception(GetLastError());

	backend->EventQueue = queue;
	backend->Context = uiContext;
	backend->EventSink = sink;

	return backend;
}

void DestroyTextBoxBackend(NativeBackend* backend)
{
	RemoveWindowSubclass(backend->Handle, Win32TextBoxProc, TEXTBOX_SUBCLASS_ID);
	DestroyControlBackend(backend);
}

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

NativeBackend* CreateLabelBackend(Label* label, NativeBackend* parentBackend, EventQueue* queue, UIContext* uiContext, IEventSink* sink)
{
	RegisterLabelClass();

	auto* backend = new NativeBackend{};
	backend->Owner = label;

	HWND parentHwnd = parentBackend ? parentBackend->Handle : nullptr;

	DWORD style = WS_CHILD | WS_CLIPSIBLINGS;

	if (label->IsVisible())
		style |= WS_VISIBLE;

	if (!label->IsEnabled())
		style |= WS_DISABLED;

	backend->Handle = CreateWindowExW(
		0,
		kLabelClassName,
		nullptr, // Text is set OnPaint()
		style,
		label->GetX(),
		label->GetY(),
		label->GetWidth(),
		label->GetHeight(),
		parentHwnd,
		nullptr,
		GetModuleHandle(nullptr),
		backend
	);

	if (!backend->Handle)
		throw Win32Exception(GetLastError());

	backend->EventQueue = queue;
	backend->Context = uiContext;
	backend->EventSink = sink;

	return backend;
}

static UINT BuildMessageBoxFlags(MessageBoxButtons buttons, MessageBoxIcon icon, MessageBoxDefaultButton def)
{
	UINT flags = 0;

	// Buttons
	switch (buttons)
	{
	case MessageBoxButtons::OK:                flags |= MB_OK; break;
	case MessageBoxButtons::OKCancel:          flags |= MB_OKCANCEL; break;
	case MessageBoxButtons::YesNo:             flags |= MB_YESNO; break;
	case MessageBoxButtons::YesNoCancel:       flags |= MB_YESNOCANCEL; break;
	case MessageBoxButtons::RetryCancel:       flags |= MB_RETRYCANCEL; break;
	case MessageBoxButtons::AbortRetryIgnore:  flags |= MB_ABORTRETRYIGNORE; break;
	}

	// Icon
	switch (icon)
	{
	case MessageBoxIcon::Info:     flags |= MB_ICONINFORMATION; break;
	case MessageBoxIcon::Warning:  flags |= MB_ICONWARNING; break;
	case MessageBoxIcon::Error:    flags |= MB_ICONERROR; break;
	case MessageBoxIcon::Question: flags |= MB_ICONQUESTION; break;
	default: break;
	}

	// Default button
	switch (def)
	{
	case MessageBoxDefaultButton::Button2: flags |= MB_DEFBUTTON2; break;
	case MessageBoxDefaultButton::Button3: flags |= MB_DEFBUTTON3; break;
	default: flags |= MB_DEFBUTTON1; break;
	}

	return flags;
}

MessageBoxResult CreateMessageBox(NativeBackend* backend, MessageBoxDescriptor* desc)
{
	return TranslateResult(
		MessageBox(
			backend != nullptr ? backend->Handle : nullptr,
			desc->Text.ToWideCharArray().GetData(),
			desc->Title.ToWideCharArray().GetData(),
			BuildMessageBoxFlags(desc->Buttons, desc->Icon, desc->DefaultButton)
		));
}

MessageBoxResult TranslateResult(i32 win32Result)
{
	switch ((int32_t)win32Result)
	{
	case IDOK:       return MessageBoxResult::OK;
	case IDCANCEL:   return MessageBoxResult::Cancel;
	case IDYES:      return MessageBoxResult::Yes;
	case IDNO:       return MessageBoxResult::No;
	case IDRETRY:    return MessageBoxResult::Retry;
	case IDABORT:    return MessageBoxResult::Abort;
	case IDIGNORE:   return MessageBoxResult::Ignore;
	case IDCLOSE:    return MessageBoxResult::Close;
	default:         return MessageBoxResult::Timeout; // 0 ou qualquer valor inesperado
	}
}