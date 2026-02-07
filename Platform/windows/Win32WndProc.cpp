#include "Win32WndProc.hpp"
#include "Win32ObjectHeader.hpp"
#include "Events/EventQueue.hpp"
#include "GUI/Core/ControlBackend.hpp"
#include "GUI/Window/WindowBackend.hpp"
#include "Events/Input/KeyboardEvents.hpp"
#include "Events/GUI/UIEvents.hpp"
#include "Events/Input/MouseEvents.hpp"
#include "Events/GUI/BehavioralEvents.hpp"
#include "Events/OS/SystemEvents.hpp"
#include "Win32KeyMap.hpp"
#include "Win32PowerMap.hpp"
#include "System/Console/Console.hpp"
#include "GUI/Window/Window.hpp"
#include "GUI/Core/Control.hpp"
#include "GUI/Drawing/Font.hpp"
#include "GUI/Context/FontManager.hpp"
#include "System/Exceptions.hpp"
#include "Win32Helpers.hpp"

#ifdef _DEBUG
#include "Win32Message.hpp"
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0601
#define WINVER       0x0601

#include <windows.h>

#ifndef _DEBUG
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

#include <imm.h>
#pragma comment(lib, "imm32.lib")

LRESULT CALLBACK Win32WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

#ifdef _DEBUG
	Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	// 🔑 SPECIAL CASE BECAUSE WM_NCCREATE RETURNS 1 WHEN SUCCESS INSTEAD OF 0
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* h = static_cast<Win32ObjectHeader*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)h);

		auto* backend = static_cast<WindowBackend*>(h);
		backend->hwnd = hwnd;
		return true;
	}

	if (!header)
		return false;

	auto* backend = static_cast<WindowBackend*>(header);
	Window* window = backend->owner;

	// 🔑 HANDLE CORRETO
	UIHandle owner = UIHandle::FromWindow(window);

	switch (msg)
	{
	case WM_NCACTIVATE:
		return 1;
	case WM_CREATE:
		return 0;

		// -------------------- Window --------------------
	case WM_NCDESTROY:
	{
		auto* backend = reinterpret_cast<WindowBackend*>(
			GetWindowLongPtr(hwnd, GWLP_USERDATA)
			);

		if (backend)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
			delete backend;
		}

		return 0;
	}
	case WM_SIZE:
	{
		uint32_t width = LOWORD(lp);
		uint32_t height = HIWORD(lp);

		// Resize sempre acontece
		backend->queue->Push(new ResizedEvent(owner, width, height));

		// Estados especiais
		switch (wp)
		{
		case SIZE_MINIMIZED:
			backend->queue->Push(new MinimizeEvent(owner));
			break;

		case SIZE_MAXIMIZED:
			backend->queue->Push(new MaximizeEvent(owner));
			break;

		case SIZE_RESTORED:
			backend->queue->Push(new RestoreEvent(owner));
			break;
		}

		return 0;
	}
	case WM_SIZING:
	{
		RECT* r = (RECT*)lp;

		ResizingEvent ev = ResizingEvent(owner, (int32_t)r->right, (int32_t)r->bottom);

		// Dispatch imediato
		backend->owner->Dispatch(ev);
		if (ev.Handled)
		{
			// bloqueia o resize
			Size z = backend->owner->GetSize();
			r->right = (int32_t)z.GetWidth();
			r->bottom = (int32_t)z.GetHeight();
			return 0;
		}

		return 0;
	}
	case WM_MOVE:
		backend->queue->Push(new MoveEvent(owner, (int)(short)LOWORD(lp), (int)(short)HIWORD(lp)));
		break;

	case WM_SETFOCUS:
		backend->queue->Push(new FocusGainedEvent(owner));
		break;

	case WM_KILLFOCUS:
		backend->queue->Push(new FocusLostEvent(owner));
		break;

	case WM_SHOWWINDOW:
	{
		if (wp)
			backend->queue->Push(new ShowEvent(owner));
		else
			backend->queue->Push(new HideEvent(owner));
		return DefWindowProc(static_cast<HWND>(owner.Handle.Get()), msg, wp, lp);;
	}
	case WM_DPICHANGED:
		backend->queue->Push(new DPIChangedEvent(owner, HIWORD(wp)));
		break;

		// -------------------- Mouse --------------------

	case WM_MOUSEMOVE:
		backend->queue->Push(new MouseMoveEvent(owner, GET_X_LPARAM(lp), GET_Y_LPARAM(lp)));
		return 0;

	case WM_LBUTTONDOWN:
		backend->queue->Push(new MouseButtonDownEvent(owner, MouseButton::Left));
		return 0;

	case WM_LBUTTONUP:
		backend->queue->Push(new MouseButtonUpEvent(owner, MouseButton::Left));
		return 0;

	case WM_RBUTTONDOWN:
		backend->queue->Push(new MouseButtonDownEvent(owner, MouseButton::Right));
		return 0;

	case WM_RBUTTONUP:
		backend->queue->Push(new MouseButtonUpEvent(owner, MouseButton::Right));
		return 0;

	case WM_MOUSEWHEEL:
		backend->queue->Push(new MouseScrollEvent(owner, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA));
		return 0;

	case WM_MOUSEHWHEEL:
		backend->queue->Push(new MouseScrollEvent(owner, (float)GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA, 0.0f));
		return 0;

		// -------------------- Keyboard --------------------

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		backend->queue->Push(new KeyDownEvent(owner, TranslateKey((uint32_t)wp, (uint32_t)lp), (lp & (1 << 30)) != 0));
		return 0;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		backend->queue->Push(new KeyUpEvent(owner, TranslateKey((uint32_t)wp, (uint32_t)lp)));
		return 0;

		// -------------------- Text / IME --------------------

	case WM_CHAR:
		backend->queue->Push(new TextInputEvent(owner, static_cast<uint32_t>(wp)));
		return 0;

		// IME reservado (implementar depois)
	case WM_IME_STARTCOMPOSITION:
	{
		backend->queue->Push(new ImeCompositionEvent(
			owner,
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
					owner,
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
					owner,
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
			owner,
			ImeCompositionType::End,
			String::Empty(),
			0
		));
		return 0;
	}
	// -------------------- System --------------------

	case WM_QUIT:
		backend->queue->Push(new SystemQuitEvent());
		break;

	case WM_ENDSESSION:
		backend->queue->Push(new SystemShutdownEvent());
		return 0;
	case WM_CLOSE:
	{
		backend->queue->Push(new CloseEvent(owner));

		// IMPORTANTE:
		// NÃO chame DestroyWindow aqui automaticamente
		// Deixe a engine decidir
		return 0;
	}
	case WM_DESTROY:
	{
		backend->queue->Push(new DestroyEvent(owner));
		return 0;
	}
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

LRESULT CALLBACK Win32LabelProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	auto* header = reinterpret_cast<Win32ObjectHeader*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

#ifdef _DEBUG
	Console::WriteLine(String::Concat("Handle: ", Pointer(hwnd).ToString(), " | Message: ", Message(msg, wp, lp).ToString()));
#endif

	// 🔑 SPECIAL CASE BECAUSE WM_NCCREATE RETURNS 1 WHEN SUCCESS INSTEAD OF 0
	if (msg == WM_NCCREATE)
	{
		auto* cs = reinterpret_cast<CREATESTRUCT*>(lp);
		auto* h = static_cast<ControlBackend*>(cs->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)h);

		auto* backend = static_cast<ControlBackend*>(h);
		backend->hwnd = hwnd;
		return true;
	}

	if (!header)
		return false;

	auto* backend = static_cast<ControlBackend*>(header);
	Control* window = backend->owner;

	// 🔑 HANDLE CORRETO
	UIHandle owner = UIHandle::FromControl(window);

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(backend->hwnd, &ps);

		RECT rc;
		GetClientRect(backend->hwnd, &rc);

		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);

		HGDIOBJ oldBmp = SelectObject(memDC, bmp);

		// Fundo
		HBRUSH brush = CreateSolidBrush(ToCOLORREF(backend->owner->GetBackgroundColor()));
		FillRect(memDC, &rc, brush);

		DeleteObject(brush);

		// Fonte
		SelectObject(memDC, backend->context->GetFontManager()->GetNativeObject(backend->owner->GetFont()).Get());

		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, ToCOLORREF(backend->owner->GetForeColor()));

		UINT drawflags = Win32ConvertTextFlags(backend->owner->GetHorizontalAlignment(), backend->owner->GetVerticalAlignment(), backend->owner->GetTextFormat());

		Array<wchar_t> buffer = backend->owner->GetText().ToWideCharArray();

		DrawTextExW(
			memDC,
			buffer.GetData(),
			buffer.GetLength(),
			&rc,
			drawflags,
			nullptr);

		BitBlt(
			hdc, 0, 0,
			rc.right, rc.bottom,
			memDC, 0, 0,
			SRCCOPY);

		SelectObject(memDC, oldBmp);
		DeleteObject(bmp);
		DeleteDC(memDC);

		EndPaint(backend->hwnd, &ps);
		return 0;
	}

	case WM_ERASEBKGND:
		return 1; // evita flicker
	case WM_DPICHANGED:
		if (backend) backend->queue->Push(new DPIChangedEvent(owner, HIWORD(wp)));
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wp, lp);
}